#ifndef FLEET_MANAGER_HPP
#define FLEET_MANAGER_HPP
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_srvs/srv/trigger.hpp"
#include <map>
#include <string>

class FleetManager : public rclcpp::Node
{
    private:
    struct DroneData
    {
        std::string battery;
        std:: string altitude;
        std::string waypoint;
        std::string status;
    };
    std::map<std::string, DroneData> fleet_data_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_status_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_status_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_status_sub_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_alert_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_alert_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_alert_sub_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_telemetry_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_telemetry_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_telemetry_sub_;

    rclcpp::TimerBase::SharedPtr report_timer_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr service_;
    
    void status_callback(const std_msgs::msg::String::SharedPtr msg);
    void alert_callback(const std_msgs::msg::String::SharedPtr msg);
    void telemetry_callback(const std_msgs::msg::String::SharedPtr msg);

    void print_report();

    void service_callback(
        const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response
    );

    public:
    FleetManager();
    ~FleetManager() noexcept = default;
};


#endif