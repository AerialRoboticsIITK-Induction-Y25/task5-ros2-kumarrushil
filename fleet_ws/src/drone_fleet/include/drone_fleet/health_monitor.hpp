#ifndef HEALTH_MONITOR_HPP
#define HEALTH_MONITOR_HPP
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <deque>
#include <map>
#include <string>

class HealthMonitor : public rclcpp::Node
{
    private:
    struct BatterySample
    {
        double battery;
        double time;
    };

    std::map<std::string, std::deque<BatterySample>> battery_history_;

    rclcpp :: Subscription<std_msgs::msg::String>::SharedPtr alpha_sub_;
    rclcpp :: Subscription<std_msgs::msg::String>::SharedPtr beta_sub_;
    rclcpp :: Subscription<std_msgs::msg::String>::SharedPtr gamma_sub_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr warning_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr summary_pub_;
    rclcpp::TimerBase::SharedPtr diagnostics_timer_;

    void telemetry_callback(const std_msgs::msg::String::SharedPtr msg);
    double calculate_drain_rate(const std::string& drone_name);
    void print_diagnostics();

    public:
    HealthMonitor();
    ~HealthMonitor() noexcept = default;

};

#endif