#include "drone_fleet/health_monitor.hpp"
using namespace std::chrono_literals;

HealthMonitor::HealthMonitor()
:Node("health_monitor")
{
    alpha_sub_ = this->create_subscription<std_msgs::msg::String>("/drone/Alpha/telemetry", 
        10, 
        std::bind(&HealthMonitor::telemetry_callback, this, std::placeholders::_1));
    
    beta_sub_ = this->create_subscription<std_msgs::msg::String>("/drone/Beta/telemetry",
            10,
            std::bind(&HealthMonitor::telemetry_callback,this,std::placeholders::_1));

    gamma_sub_ = this->create_subscription<std_msgs::msg::String>("/drone/Gamma/telemetry",
            10,
            std::bind(&HealthMonitor::telemetry_callback,this,std::placeholders::_1));

    warning_pub_ =this->create_publisher<std_msgs::msg::String>(
            "/fleet/health_warning",
            10);

    summary_pub_ =
        this->create_publisher<std_msgs::msg::String>(
            "/fleet/health_summary",
            10);

    diagnostics_timer_ =
        this->create_wall_timer(
            10s,
            std::bind(&HealthMonitor::print_diagnostics,this));
}

void HealthMonitor::telemetry_callback(const std_msgs::msg::String::SharedPtr msg)
{
    std::string data = msg->data;
    size_t name_start = data.find("\"name\":\"") +8;
    size_t name_end = data.find("\"", name_start);
    std::string drone_name = data.substr(name_start, name_end-name_start);
    size_t battery_start =data.find("\"battery\":") + 10;
    size_t battery_end =data.find(",", battery_start);
    double battery =std::stod(data.substr(battery_start,battery_end - battery_start));
    double current_time = this->now().seconds();

    battery_history_[drone_name].push_back({battery, current_time});

    if (battery_history_[drone_name].size()>10)
    {
        battery_history_[drone_name].pop_front();
    }

    double drain_rate = calculate_drain_rate(drone_name);
    if(drain_rate > 1.5)
    {
        std_msgs::msg::String warning_msg;
        warning_msg.data= "WARNING: " + drone_name + "drain rate critical";
        warning_pub_->publish(warning_msg);
        RCLCPP_WARN(this->get_logger(), "%s", warning_msg.data.c_str());
    }
}

double HealthMonitor::calculate_drain_rate(const std::string& drone_name)
{
    auto& history = battery_history_[drone_name];
    if (history.size()<2)
    {
        return 0.0;
    }

    double first_battery= history.front().battery;
    double last_battery =history.back().battery;
    double first_time =history.front().time;
    double last_time =history.back().time;
    double battery_diff =first_battery - last_battery;
    double time_diff =last_time - first_time;
    if (time_diff <= 0.0)
    {
        return 0.0;
    }

    return battery_diff / time_diff;
}

void HealthMonitor :: print_diagnostics()
{
    std::cout<< "\n HEALTH DIAGNOSTICS \n";
    std::cout<< "Drone\tDrainRate\tTimeToCritical\tTimeToZero\n";
    std_msgs::msg::String summary_msg;
    summary_msg.data = "{";
    bool first = true;
    for(auto& drone : battery_history_)
    {
        std::string name= drone.first;
        double drain_rate= calculate_drain_rate(name);
        double current_battery = drone.second.back().battery;
        double time_to_critical = 0.0;
        double time_to_zero = 0.0;

        if (drain_rate > 0.0)
        {
            time_to_critical = (current_battery - 20.0)/drain_rate;
            time_to_zero = current_battery / drain_rate;
        }
        std::cout
            << name << "\t"
            << drain_rate << "\t\t"
            << time_to_critical << "\t\t"
            << time_to_zero << "\n";
        
        if (!first)
        {
            summary_msg.data += ",";
        }

        summary_msg.data +=
            "\"" + name + "\":{"
            "\"drain_rate\":" +
            std::to_string(drain_rate) + ","
            "\"time_to_critical\":" +
            std::to_string(time_to_critical) + ","
            "\"time_to_zero\":" +
            std::to_string(time_to_zero) +
            "}";

        first = false;
    }
    summary_msg.data += "}";
    summary_pub_->publish(summary_msg);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthMonitor>());
    rclcpp::shutdown();
}