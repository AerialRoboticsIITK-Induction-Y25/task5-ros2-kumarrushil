#ifndef DRONE_HPP
#define DRONE_HPP

#include "Exceptions.hpp"
#include "Vehicle.hpp"
#include <iostream>    
#include <string>
using namespace std;
#include <vector>

class Drone: public Vehicle
{
protected:
    float altitude;
    float max_altitude;
private:
    float speed;
public:
    Drone(string name, float battery, float alt, float maxalt, float sp) : Vehicle(name, battery)
    {
        altitude=alt;
        max_altitude=maxalt;
        speed=sp;
    } 
    void take_off(float target_altitude)
    {
        if (target_altitude>max_altitude)
        throw AltitudeError();
        else
        {
            altitude= target_altitude;
            set_status("flying");
            auto now = chrono::system_clock::now();
            time_t current_time = chrono::system_clock::to_time_t(now);
            string timestamp = ctime(&current_time);
            add_flight_log(timestamp +"Flying status changed to "+get_status());   
        }
    }
    void land()
    {
        altitude=0;
        set_status("idle");
        auto now = chrono::system_clock::now();
        time_t current_time = chrono::system_clock::to_time_t(now);
        string timestamp = ctime(&current_time);    
        add_flight_log(timestamp +"Flying status changed to "+get_status()+ "\nDrone landed");
    }
    void emergency_stop()
    {
        speed=0;
        float bat =get_battery_level();
        set_battery_level(bat-30);
        set_status("idle");
        auto now = chrono::system_clock::now();
        time_t current_time = chrono::system_clock::to_time_t(now);
        string timestamp = ctime(&current_time);    
        add_flight_log(timestamp +"Flying status changed to "+get_status()+ "\nEmergency stop");
    }
    void get_info()
    {
        cout << "Name: " << name << endl;
        cout << "Battery: " << get_battery_level() << endl;
        cout << "Altitude: " << altitude << endl;
        cout << "Speed: " << speed << endl;
        cout << "Status: " << get_status() << endl;
    }
    float get_speed()
    {
        return speed;
    }
    float get_altitude()
    {
        return altitude;
    }

};

#endif