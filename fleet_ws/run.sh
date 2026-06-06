#!/bin/bash

docker build -t drone_fleet .

docker run -it --rm \
--net=host \
drone_fleet
bash -c "source /opt/ros/jazzy/setup.bash && source /workspace/install/setup.bash && ros2 launch drone_fleet fleet.launch.py"