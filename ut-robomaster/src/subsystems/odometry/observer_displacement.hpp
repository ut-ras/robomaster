#ifndef SUBSYSTEMS_ODOMETRY_OBSERVER_DISPLACEMENT_HPP_
#define SUBSYSTEMS_ODOMETRY_OBSERVER_DISPLACEMENT_HPP_

#include "tap/algorithms/odometry/chassis_displacement_observer_interface.hpp"

#include "drivers.hpp"

namespace subsystems::odometry
{
using tap::algorithms::odometry::ChassisDisplacementObserverInterface;
using namespace tap::communication::sensors::imu;

class ChassisDisplacementObserver : public ChassisDisplacementObserverInterface
{
public:
    ChassisDisplacementObserver(src::Drivers* drivers);
    bool getVelocityChassisDisplacement(
        modm::Vector3f* const velocity,
        modm::Vector3f* const displacement) const override final;

private:
    bmi088::Bmi088* const imu;

    // Used for calculation purposes for Velocity Verlet, hence mutable
    mutable modm::Vector3f lastAcc;   // m/s^2
    mutable modm::Vector3f lastVel;   // m/s
    mutable modm::Vector3f lastDisp;  // m
    mutable uint32_t lastTime;        // ms
};
}  // namespace subsystems::odometry

#endif  // SUBSYSTEMS_ODOMETRY_CHASSIS_DISPLACEMENT_OBSERVER_HPP_