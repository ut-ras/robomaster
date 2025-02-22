#include "odometry_subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/subsystem.hpp"

namespace subsystems::odometry
{
OdometrySubsystem::OdometrySubsystem(
    src::Drivers* drivers,
    ChassisSubsystem* chassis,
    TurretSubsystem* turret)
    : Subsystem(drivers),
      drivers(drivers),
      chassis(chassis),
      turret(turret),
      chassisDisplacement(drivers, chassis),
      chassisYaw(drivers),
      chassisTracker(&chassisYaw, &chassisDisplacement) {};

void OdometrySubsystem::initialize() {};

void OdometrySubsystem::refresh()
{
    Vector3f lastAcc;  // m/s^2

    setAmputated(!hardwareOk());
    if (!isAmputated())
    {
        chassisTracker.update();
    }

    // Vector3f velocity = chassis->measureVelocity();   // m/s^2
    // chassisDisplacement.getVelocityChassisDisplacement(&velocity, &lastAcc);

    if (refreshTimer.execute())
    {

        drivers->rtt.plot(3, getPosition().getX()); //problematic function
        drivers->rtt.plot(2, getPosition().getY()); //problematic function
        drivers->rtt.plot(1, getLinearVelocity().getX());
    }
}

bool OdometrySubsystem::hardwareOk() { return chassis->hardwareOk() && turret->hardwareOk(); }

Vector2f OdometrySubsystem::getPosition()
{
    return chassisTracker.getCurrentLocation2D().getPosition();
}
Vector2f OdometrySubsystem::getLinearVelocity() { return chassisTracker.getCurrentVelocity2D(); }

float OdometrySubsystem::getChassisYaw() { return chassisTracker.getYaw(); }
float OdometrySubsystem::getChassisAngularVelocity() { return chassis->measureVelocity().z; }

float OdometrySubsystem::getTurretLocalYaw() { return turret->getCurrentLocalPitch(); }
float OdometrySubsystem::getTurretLocalPitch() { return turret->getCurrentLocalYaw(); }
}  // namespace subsystems::odometry