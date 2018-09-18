#!/bin/bash
source /opt/ros/kinetic/setup.bash
source /home/amsl/ros_catkin_ws/devel/setup.bash

source /home/amsl/.bashrc

# roscore
gnome-terminal -e "/opt/ros/kinetic/bin/roscore" --geometry=50x12+0+0 &
sleep 1s

# use_sim_time true
gnome-terminal -e "rosparam set use_sim_time true" --geometry=50x12+0+0 &
sleep 1s

# tf
gnome-terminal -e "/opt/ros/kinetic/bin/roslaunch calibration sq_lidar2realsense.launch" --geometry=50x12+0+250 &
sleep 1s

gnome-terminal -e "/opt/ros/kinetic/bin/roslaunch calibration realsense0_dumy_tf.launch" --geometry=50x12+0+500 &
sleep 1s

# coloring
gnome-terminal -e "/opt/ros/kinetic/bin/roslaunch calibration coloring.launch" --geometry=50x12+0+750 &
sleep 1s

# evaluate
gnome-terminal -e "/opt/ros/kinetic/bin/roslaunch calibration calibration_evaluate.launch" --geometry=50x12+600+0 &
sleep 1s

# rqt_reconfigure
gnome-terminal -e "/opt/ros/kinetic/bin/rosrun rqt_reconfigure rqt_reconfigure" --geometry=50x12+600+250 &
sleep 1s

# bagfile
gnome-terminal -e "/opt/ros/kinetic/bin/rosbag play /home/amsl/bagfiles/2018/IRC/calibration_experiments_0907.bag --clock -l" --geometry=50x12+600+250 &
sleep 1s

# rviz
gnome-terminal -e '/opt/ros/kinetic/bin/rosrun rviz rviz -d /home/amsl/.rviz/calibration_evaluate.rviz' --geometry=50x12+600+500 &
sleep 1s
