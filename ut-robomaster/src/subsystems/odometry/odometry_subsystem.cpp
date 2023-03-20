#include "odometry_subsystem.hpp"

#include "robots/robot_constants.hpp"
namespace subsystems
{
namespace odometry
{
OdometrySubsystem::OdometrySubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      chassisDisplacement(drivers),
      chassisYaw(drivers),
      tracker(&chassisYaw, &chassisDisplacement){};

void OdometrySubsystem::initialize(){};
void OdometrySubsystem::refresh() { tracker.update(); };
};  // namespace odometry
}  // namespace subsystems