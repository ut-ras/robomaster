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
      chassisTracker(&chassisYaw, &chassisDisplacement){};

void OdometrySubsystem::initialize(){};
void OdometrySubsystem::refresh() { chassisTracker.update(); }

Vector2f OdometrySubsystem::getPosition()
{
    return chassisTracker.getCurrentLocation2D().getPosition();
}
Vector2f OdometrySubsystem::getLinearVelocity() { return chassisTracker.getCurrentVelocity2D(); }

float OdometrySubsystem::getChassisYaw() { return chassisTracker.getYaw(); }
float OdometrySubsystem::getChassisAngularVelocity() { return chassis->measureVelocity().z; }

float OdometrySubsystem::getTurretLocalYaw() { return turret->getCurrentLocalPitch(); }
float OdometrySubsystem::getTurretLocalPitch() { return turret->getCurrentLocalYaw(); }
};  // namespace odometry
}  // namespace subsystems