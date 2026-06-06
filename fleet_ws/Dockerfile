FROM osrf/ros:jazzy-desktop AS builder

WORKDIR /workspace

RUN apt-get update && apt-get install -y \
python3-colcon-common-extensions \
ros-jazzy-rclcpp \
ros-jazzy-std-msgs \
ros-jazzy-std-srvs

COPY . /workspace

SHELL ["/bin/bash", "-c"]

RUN source /opt/ros/jazzy/setup.bash && \
colcon build 

FROM osrf/ros:jazzy-desktop AS runtime

WORKDIR /workspace

COPY --from=builder /workspace/install /workspace/install

SHELL ["/bin/bash", "-c"]

RUN echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc && \
    echo "source /workspace/install/setup.bash" >> ~/.bashrc

ENTRYPOINT ["/bin/bash", "-c"]

CMD ["source /opt/ros/jazzy/setup.bash && \
source /workspace/install/setup.bash && \
ros2 launch drone_fleet fleet.launch.py"]
