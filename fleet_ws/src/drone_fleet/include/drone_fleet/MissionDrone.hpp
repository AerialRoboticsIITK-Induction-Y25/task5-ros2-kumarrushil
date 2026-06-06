#ifndef MISSIONDRONE_HPP
#define MISSIONDRONE_HPP

#include "Exceptions.hpp"
#include "Vehicle.hpp"
#include "Drone.hpp"
#include <iostream>    
#include <string>
using namespace std;
#include <vector>

class MissionDrone : public Drone
{
public:
    string mission_name;
    vector<tuple<float, float, float>> waypoints;
    int current_waypoint_index;
private:
    vector<pair<tuple<float, float, float>, string>> visited_waypoints;
public:
    MissionDrone(string name, float battery, float altitude, float max_altitude, float speed, string nm, int current) : Drone(name, battery, altitude, max_altitude, speed)
    {
        mission_name=nm;
        current_waypoint_index=current;

    }
    tuple<float, float, float> next_waypoint()
    {
        float bat= get_battery_level();
        set_battery_level(bat-1.5);
        return waypoints[current_waypoint_index];
    }
    void skip_waypoint(const string& reason)
    {
        current_waypoint_index+=1;
        cout << "Reason: " << reason << endl;
    }
    bool mission_complete()
    {
        if (current_waypoint_index== static_cast<int>(waypoints.size())-1)
        return true;
        else
        return false;
    }
    string mission_summary()
    {return "";}
    void get_info()
    {
        Drone::get_info();
        cout << "Mission name: " << mission_name << endl;
        cout << "Current waypoint index: " << current_waypoint_index << endl;
    }
    void get_visited_waypoints()
    {
        for(size_t i=0; i< visited_waypoints.size(); i++)
        {
            float x = get<0>(visited_waypoints[i].first);
            float y = get<1>(visited_waypoints[i].first);
            float z = get<2>(visited_waypoints[i].first);
            string time = visited_waypoints[i].second;
            cout << "Waypoint timestamp: " << time << endl;
            cout << "Coordinates: ("<< x << ", "<< y << ", "<< z << ")" << endl;
        }
    }

};

#endif