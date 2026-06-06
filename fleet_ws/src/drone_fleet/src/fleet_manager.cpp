#include "drone_fleet/fleet_manager.hpp"
using namespace std::chrono_literals;

FleetManager::FleetManager()
: Node("fleet_manager")
{
    alpha_status_sub_= this->create_subscription<std_msgs::msg::String>("/drone/Alpha/status",
        10, 
        std::bind(&FleetManager::status_callback, this, std::placeholders::_1));
    
    beta_status_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Beta/status",
            10,
            std::bind(&FleetManager::status_callback, this, std::placeholders::_1));

    gamma_status_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Gamma/status",
            10,
            std::bind(&FleetManager::status_callback, this, std::placeholders::_1));

    alpha_alert_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Alpha/alert",
            10,
            std::bind(&FleetManager::alert_callback, this, std::placeholders::_1));

    beta_alert_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Beta/alert",
            10,
            std::bind(&FleetManager::alert_callback, this, std::placeholders::_1));

    gamma_alert_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Gamma/alert",
            10,
            std::bind(&FleetManager::alert_callback, this, std::placeholders::_1));

    alpha_telemetry_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Alpha/telemetry",
            10,
            std::bind(&FleetManager::telemetry_callback, this, std::placeholders::_1));

    beta_telemetry_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Beta/telemetry",
            10,
            std::bind(&FleetManager::telemetry_callback, this, std::placeholders::_1));

    gamma_telemetry_sub_ =
        this->create_subscription<std_msgs::msg::String>(
            "/drone/Gamma/telemetry",
            10,
            std::bind(&FleetManager::telemetry_callback, this, std::placeholders::_1));

    report_timer_= this->create_wall_timer(5s,std::bind(&FleetManager::print_report, this));

    service_= this->create_service<std_srvs::srv::Trigger>(
        "/fleet/status_report", 
        std::bind(&FleetManager::service_callback, this, std::placeholders::_1, std::placeholders::_2)
    );

}

void FleetManager::status_callback(const std_msgs::msg::String::SharedPtr msg)
{
    std::string data= msg->data;
    std::string drone_name;
    size_t start= data.find("name:") +5;
    size_t end = data.find("|battery:");
    drone_name= data.substr(start, end-start);
    fleet_data_[drone_name].battery =
        data.substr(data.find("|battery:") + 9,
        data.find("|altitude:") - (data.find("|battery:") + 9));

    fleet_data_[drone_name].altitude =
        data.substr(data.find("|altitude:") + 10,
        data.find("|status:") - (data.find("|altitude:") + 10));

    fleet_data_[drone_name].status =
        data.substr(data.find("|status:") + 8,
        data.find("|waypoint:") - (data.find("|status:") + 8));

    fleet_data_[drone_name].waypoint =
        data.substr(data.find("|waypoint:") + 1,
        data.find("|speed:") - (data.find("|waypoint:") + 1));

}

void FleetManager :: alert_callback(const std_msgs::msg::String::SharedPtr msg)
{
    RCLCPP_WARN(this->get_logger(), "ALERT RECEIVED: %s", msg->data.c_str());
}

void FleetManager::telemetry_callback(const std_msgs::msg::String::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(),"Telemetry: %s",msg->data.c_str());
}

void FleetManager:: print_report()
{
    std::cout << "\nFLEET REPORT\n";

    std::cout<< "Drone\tBattery\tAltitude\tWaypoint\tStatus\n";

    for (auto & drone : fleet_data_)
    {
        std::cout
            << drone.first << "\t"
            << drone.second.battery << "\t"
            << drone.second.altitude << "\t\t"
            << drone.second.waypoint << "\t\t"
            << drone.second.status << "\n";
    }

}

void FleetManager::service_callback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
  std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
    (void)request;
    print_report();
    response->success = true;
    response->message = "Fleet report generated";
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FleetManager>());
    rclcpp::shutdown();
}
