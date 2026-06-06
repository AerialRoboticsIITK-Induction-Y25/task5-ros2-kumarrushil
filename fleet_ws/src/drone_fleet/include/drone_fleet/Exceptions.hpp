#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <string>
#include <exception>
using namespace std;

class BaseDroneException : public exception
{
    protected:
    string message;
    public:
    const char* what() const noexcept override
    {
        return "Drone exception occurred.";
    }

};
class BatteryDepletedError : public BaseDroneException
{
public:
    const char* what() const noexcept override
    {
        return "Battery depleted.";
    }
};

class InvalidStateError : public BaseDroneException
{
public:
    const char* what() const noexcept override
    {
        return "Invalid state.";
    }
};

class AltitudeError : public BaseDroneException
{
public:
    const char* what() const noexcept override
    {
        return "Altitude error.";
    }
};

#endif