#ifndef SUBSYSTEMS_ODOMETRY_OBSERVER_YAW_WORLD_HPP_
#define SUBSYSTEMS_ODOMETRY_OBSERVER_YAW_WORLD_HPP_

#include "tap/algorithms/odometry/chassis_world_yaw_observer_interface.hpp"

#include "modm/math/geometry.hpp"
#include "modm/math/geometry/angle.hpp"
#include "modm/math/geometry/location_2d.hpp"
#include "robots/standard/robot_comms.hpp"

#include "drivers.hpp"

namespace subsystems::odometry
{
using tap::algorithms::odometry::ChassisWorldYawObserverInterface;
using namespace tap::communication::sensors::imu;

class ChassisWorldYawObserver : public ChassisWorldYawObserverInterface
{
public:
    ChassisWorldYawObserver(src::Drivers* drivers);
    const char* getName() { return "Chassis World Yaw Observer"; }
    bool getChassisWorldYaw(float* yaw) const override final;

private:
    src::Drivers* drivers;
    bmi088::Bmi088* const imu;
};

};  // namespace subsystems::odometry

#endif