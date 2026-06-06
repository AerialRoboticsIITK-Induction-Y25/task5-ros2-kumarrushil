#ifndef DRONE_NODE_HPP
#define DRONE_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "drone_fleet/MissionDrone.hpp"

class DroneNode : public rclcpp::Node
{
private:
    MissionDrone drone_;

    std::string drone_name_;

    int publish_count_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr status_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mission_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr telemetry_pub_;

    rclcpp::TimerBase::SharedPtr status_timer_;
    rclcpp::TimerBase::SharedPtr telemetry_timer_;

    void publish_status();
    void publish_telemetry();

public:
    DroneNode();
};

#endif