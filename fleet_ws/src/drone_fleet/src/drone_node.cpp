#include "drone_fleet/drone_node.hpp"
using namespace std::chrono_literals;
DroneNode::DroneNode(): Node("drone_node"), drone_("Alpha", 100.0, 0.0, 500.0, 10.0, "Mission1", 0), publish_count_(0)
{
    this->declare_parameter("drone_name", "Alpha");
    this->declare_parameter("initial_battery", 100.0);
    this->declare_parameter("mission_name", "Mission1");

    drone_name_ = this->get_parameter("drone_name").as_string();

    double battery = this->get_parameter("initial_battery").as_double();

    std::string mission = this->get_parameter("mission_name").as_string();
    drone_ = MissionDrone(drone_name_,battery,0.0,500.0,5.0,mission,0);

    drone_.waypoints.push_back({10.0, 20.0, 30.0});
    drone_.waypoints.push_back({40.0, 50.0, 60.0});
    drone_.waypoints.push_back({70.0, 80.0, 90.0});
    drone_.waypoints.push_back({45.0, 65.0, 44.0});
    drone_.waypoints.push_back({99.0, 10.5, 43.5});

    drone_.take_off(50.0);

    status_pub_ =this->create_publisher<std_msgs::msg::String>("/drone/" + drone_name_ + "/status", 10);
    alert_pub_ =this->create_publisher<std_msgs::msg::String>("/drone/" + drone_name_ + "/alert", 10);
    mission_pub_ =this->create_publisher<std_msgs::msg::String>("/drone/" + drone_name_ + "/mission_complete", 10);
    telemetry_pub_ =this->create_publisher<std_msgs::msg::String>("/drone/" + drone_name_ + "/telemetry", 10);
    
    status_timer_ =this->create_wall_timer(1s,std::bind(&DroneNode::publish_status, this));
    telemetry_timer_ =this->create_wall_timer(2s,std::bind(&DroneNode::publish_telemetry, this));
}


void DroneNode :: publish_status()
{
    publish_count_++;
    drone_.drain_battery(0.5);
    if(publish_count_%3==0)
    {
        if (!drone_.mission_complete())
        {
            drone_.current_waypoint_index++;
        }
    }
    std_msgs::msg::String msg;

    msg.data =
        "name:" + drone_name_ +
        "|battery:" + std::to_string(drone_.get_battery_level()) +
        "|altitude:" + std::to_string(drone_.get_altitude()) +
        "|status:" + drone_.get_status() +
        "|waypoint:" +
        std::to_string(drone_.current_waypoint_index + 1) +
        "/" +
        std::to_string(drone_.waypoints.size()) +
        "|speed:"+std::to_string(drone_.get_speed());

    status_pub_->publish(msg);

    if (drone_.is_critical())
    {
        std_msgs::msg::String alert_msg;
        alert_msg.data ="CRITICAL BATTERY: " + drone_name_;
        alert_pub_->publish(alert_msg);
        drone_.land();
    }

    if (drone_.mission_complete())
    {
        std_msgs::msg::String mission_msg;
        mission_msg.data =drone_name_ + " mission completed";
        mission_pub_->publish(mission_msg);
        drone_.current_waypoint_index = 0;
    }

}
void DroneNode::publish_telemetry()
{
    std_msgs::msg::String msg;
    msg.data =
        "{"
        "\"name\":\"" + drone_name_ + "\","
        "\"battery\":" +
        std::to_string(drone_.get_battery_level()) + ","
        "\"altitude\":" +
        std::to_string(drone_.get_altitude()) + ","
        "\"status\":\"" +
        drone_.get_status() + "\""
        "}";
    telemetry_pub_->publish(msg);
}
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DroneNode>());
    rclcpp::shutdown();
}