#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Exceptions.hpp"
#include <iostream>  
#include <chrono>  
#include <string>
using namespace std;
#include <vector>

class Vehicle
{
public:
    string name;
private:
    float battery;
    string status;
    vector<string> flight_log;
public:
    Vehicle(string nm, float bat)
    {
        name=nm;
        battery=bat;
        status= "Idle";
    }
    virtual void get_info()=0;
    void drain_battery(float amount)
    {
        if (battery <=0)
        throw BatteryDepletedError();
        else{
        battery-=amount;
        if (battery<0)
        battery=0;
        }

    }
    void charge_battery(float amount, int duration_seconds)
    {
        (void)duration_seconds;
        if (status != "charging")
        throw InvalidStateError();
        else{
            battery+=amount;
            if (battery>100)
            battery=100;
        }

    }
    bool is_critical()
    {
        return battery<20.0;
    }
    void get_flight_log()
    {
        cout << "\n====== FLIGHT LOG ======\n";

        for(size_t i = 0; i < flight_log.size(); i++) {

            cout << flight_log[i] << endl;
        }
    }
    void add_flight_log(string flight)
    {
        flight_log.push_back(flight);
    }
    string get_name()
    {
        return name;
    }

    float get_battery_level()
    {
        return battery;
    }

    void set_battery_level(float new_bat)
    {
        battery= new_bat;
    }

    string get_status()
    {
        return status;
    }

    void set_status(string s)
    {
        status= s;
        auto now = chrono::system_clock::now();
        time_t current_time = chrono::system_clock::to_time_t(now);
        string timestamp = ctime(&current_time);
        flight_log.push_back(timestamp +"Flying status changed to "+status);
    }
    virtual ~Vehicle() = default;
};


#endif