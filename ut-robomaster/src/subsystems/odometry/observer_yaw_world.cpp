#include "observer_yaw_world.hpp"

namespace subsystems::odometry
{
ChassisWorldYawObserver::ChassisWorldYawObserver(src::Drivers* drivers)
    : drivers(drivers),
      imu(&drivers->bmi088)
{
}
bool ChassisWorldYawObserver::getChassisWorldYaw(float* yaw) const { return false; };
};  // namespace subsystems::odometry