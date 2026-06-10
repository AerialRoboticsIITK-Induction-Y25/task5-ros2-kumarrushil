#include "drone_fleet/Exceptions.hpp"
#include "drone_fleet/Vehicle.hpp"
#include "drone_fleet/Drone.hpp"
#include "drone_fleet/MissionDrone.hpp"
#include "drone_fleet/AutonomousDrone.hpp"
#include <iostream>  
#include <cmath>  
#include <string>
using namespace std;
#include <vector>

int main()
{
    vector<Vehicle*> objects;
    Drone v1("Vakaz", 100.0, 120.0, 500.0, 45.0);
    v1.get_info();
    MissionDrone md1("Vakaz", 100.0, 120.0, 500.0, 45.0, "Alpha", 0);
    md1.get_info();
    AutonomousDrone ad1("Vakaz", 100.0, 120.0, 500.0, 45.0, "Alpha", 2, "auto", {0,0,0});
    ad1.get_info();
    objects.push_back(&v1);
    objects.push_back(&md1);
    objects.push_back(&ad1);
    // float bat= v1.battery; battery is a private member of vehicle class so an "inaccessible" error occurs
    ad1.drain_battery(10.0);
    ad1.take_off(200.0);

    ad1.waypoints.push_back({10.0, 20.0, 50.0});
    ad1.waypoints.push_back({30.0, 40.0, 70.0});
    ad1.waypoints.push_back({60.0, 80.0, 100.0});
    cout << "\n======= MISSION START =======\n";
    for (size_t i = 0; i < ad1.waypoints.size(); i++)
    {
        cout << "\nTravelling to Waypoint " << i + 1 << endl;
        cout << "(" << get<0>(ad1.waypoints[i]) << ", "<< get<1>(ad1.waypoints[i]) << ", "<< get<2>(ad1.waypoints[i])<< ")" << endl;
        ad1.drain_battery(10.0);
        if (i == 1)
        {
            ad1.detect_obstacle({20.0, 30.0, 50.0},"high");
        }
    }
    cout << "\n======= MISSION SUMMARY =======\n";
    ad1.get_info();
    ad1.get_flight_log();
    cout << "Remaining Battery: " << ad1.get_battery_level() << "%" << endl;

}