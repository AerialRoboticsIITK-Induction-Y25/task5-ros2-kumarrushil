#ifndef AUTONOMOUSDRONE_HPP
#define AUTONOMOUSDRONE_HPP

#include "Exceptions.hpp"
#include "Vehicle.hpp"
#include "Drone.hpp"
#include "MissionDrone.hpp"
#include <iostream>  
#include <cmath>  
#include <string>
using namespace std;
#include <vector>

class AutonomousDrone : public MissionDrone
{
public:
    string ai_mode;
    tuple<float, float, float> home_position;
private:
    vector<string> obstacle_log;
public: 
    void set_ai_mode(const string &mode)
    {
        ai_mode=mode;
        if (ai_mode== "return_home")
        waypoints.push_back(home_position);
    }
    AutonomousDrone(string name, float battery, float altitude, float max_altitude, float speed, string nm, int current, string mode, tuple<float, float, float> hp) : MissionDrone(name, battery, altitude, max_altitude, speed, nm, current)
    {
        ai_mode= mode;
        home_position=hp;
    }
    void detect_obstacle (tuple<float, float, float> position, const string& severity)
    {
        cout << "\n ======== OBSTACLES ======== \n";
        for (size_t i=0; i< obstacle_log.size(); i++)
        {
            cout << obstacle_log[i] << endl;
        }
        if (severity== "high")
        emergency_stop();
    }
    void auto_replan(const vector<tuple<float,float,float>>& obstacles)
    {
    vector<tuple<float, float, float>> new_waypoints;
    for (size_t i = 0; i < waypoints.size(); i++)
    {
        float wx = get<0>(waypoints[i]);
        float wy = get<1>(waypoints[i]);
        float wz = get<2>(waypoints[i]);
        bool collision = false;
        for (size_t j = 0; j < obstacles.size(); j++)
        {
            float ox = get<0>(obstacles[j]);
            float oy = get<1>(obstacles[j]);
            float oz = get<2>(obstacles[j]);
            float distance = sqrt(pow(wx - ox, 2) +pow(wy - oy, 2) +pow(wz - oz, 2));
            if (distance < 5.0)
            {
                collision = true;
                break;
            }
            
        }
        if (!collision)
        {
            new_waypoints.push_back(waypoints[i]);
        }
        
    }}
    void get_info()
    {
        MissionDrone::get_info();
        cout << "Mode: " << ai_mode << endl;
        cout << "Home location: (" << get<0>(home_position) << ", " << get<1>(home_position) << ", " << get<2>(home_position) << ")" << endl;
    }
};

#endif
