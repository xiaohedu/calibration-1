/*
 * author : Yudai Sadakuni
 *
 * detect calibration board (camera)
 *
 */

#include <calibration/function.h>

typedef pcl::PointXYZ PointA;
typedef pcl::PointCloud<PointA> CloudA;
typedef pcl::PointCloud<PointA>::Ptr CloudAPtr;

using namespace std;

// このパラメータで取得する点群のエリアを確定する
// キャリブレーションボードが収まる範囲に調整すること
double MIN_Z, MAX_Z;
double MIN_DIS, MAX_DIS;
double MIN_ANG, MAX_ANG;

// edge detection用
double RADIUS;
int SIZE;
double DISTANCE;
double TORELANCE;
int MIN_SIZE, MAX_SIZE;

ros::Publisher pub_pickup;
ros::Publisher pub_outlier;
ros::Publisher pub_edge;
ros::Publisher pub_edge_circle;
ros::Publisher pub_cluster;
ros::Publisher pub_centroid;

void pcCallback(const sensor_msgs::PointCloud2ConstPtr& msg)
{
    CloudAPtr input(new CloudA);
    pcl::fromROSMsg(*msg, *input);

    // Pickup Cloud
    CloudAPtr area(new CloudA);
    pickup_cloud<CloudAPtr>(input,
                            area,
                            MIN_Z,
                            MAX_Z,
                            MIN_ANG,
                            MAX_ANG,
                            MIN_DIS,
                            MAX_DIS);

    // edge detection
    CloudAPtr edge(new CloudA);
    edge_detection<PointA, CloudA, CloudAPtr>(area, edge, RADIUS, SIZE);

    pub_cloud(area, msg->header, pub_pickup);
    pub_cloud(edge, msg->header, pub_edge);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ransac_lidar");
    ros::NodeHandle nh("~");

    nh.param<double>("min_z",   MIN_Z,   -0.45);
    nh.param<double>("max_z",   MAX_Z,   1.0);
    nh.param<double>("min_ang", MIN_ANG, -0.785);
    nh.param<double>("max_ang", MAX_ANG, 0.785);
    nh.param<double>("min_dis", MIN_DIS, 1.0);
    nh.param<double>("max_dis", MAX_DIS, 3.0);
    nh.param<double>("radius",  RADIUS,  0.05);
    nh.param<int>("size", SIZE, 70);
    nh.param<double>("distance", DISTANCE, 0.35);
    nh.param<double>("tolerance", TORELANCE, 0.03);
    nh.param<int>("min_size", MIN_SIZE, 10);
    nh.param<int>("max_size", MAX_SIZE, 100);

    ros::Subscriber sub = nh.subscribe("/cloud", 10, pcCallback);

    pub_pickup      = nh.advertise<sensor_msgs::PointCloud2>("/camera/pickup", 10);
    pub_outlier     = nh.advertise<sensor_msgs::PointCloud2>("/camera/outlier", 10);
    pub_edge        = nh.advertise<sensor_msgs::PointCloud2>("/camera/edge", 10);
    pub_edge_circle = nh.advertise<sensor_msgs::PointCloud2>("/camera/edge_circle", 10);
    pub_cluster     = nh.advertise<sensor_msgs::PointCloud2>("/camera/cluster", 10);
    pub_centroid    = nh.advertise<sensor_msgs::PointCloud2>("/camera/centroid", 10);
   
	ros::spin();

    return 0;
}
