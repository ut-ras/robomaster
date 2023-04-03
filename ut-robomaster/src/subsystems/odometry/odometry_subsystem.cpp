#include "odometry_subsystem.hpp"

#include "robots/robot_constants.hpp"
namespace subsystems
{
namespace odometry
{
OdometrySubsystem::OdometrySubsystem(
    src::Drivers* drivers,
    ChassisSubsystem* chassis,
    TurretSubsystem* turret)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
      chassis(chassis),
      turret(turret),
      chassisDisplacement(drivers, chassis),
      chassisYaw(drivers),
      tracker(&chassisYaw, &chassisDisplacement){};

void OdometrySubsystem::initialize(){};
void OdometrySubsystem::refresh() { tracker.update(); }

Vector2f OdometrySubsystem::getPosition() { return tracker.getCurrentLocation2D().getPosition(); }
float OdometrySubsystem::getYaw() { return tracker.getYaw(); }
Vector2f OdometrySubsystem::getChassisLinearVelocity() { return tracker.getCurrentVelocity2D(); }
float OdometrySubsystem::getChassisAngularVelocity() { return chassis->measureVelocity().z; }
float OdometrySubsystem::getTurretYaw()
{
    return 0.0f;  // TODO
}
float OdometrySubsystem::getTurretPitch()
{
    return 0.0f;  // TODO
};
};  // namespace odometry
}  // namespace subsystems