#include "observer_yaw_world.hpp"

namespace subsystems::odometry
{
ChassisWorldYawObserver::ChassisWorldYawObserver(src::Drivers* drivers) : drivers(drivers) {}

bool ChassisWorldYawObserver::getChassisWorldYaw(float* yaw) const
{
    bmi088::Bmi088* imu = &drivers->bmi088;
    *yaw = modm::toRadian(imu->getYaw() - 180.0f);

    return imu->getImuState() == ImuInterface::ImuState::IMU_CALIBRATED;
};
};  // namespace subsystems::odometry