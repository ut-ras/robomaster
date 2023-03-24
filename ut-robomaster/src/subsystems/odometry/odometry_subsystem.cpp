#include "odometry_subsystem.hpp"

#include "robots/robot_constants.hpp"
namespace subsystems
{
namespace odometry
{
OdometrySubsystem::OdometrySubsystem(src::Drivers* drivers, ChassisSubsystem* chassis)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
      chassis(chassis),
      chassisDisplacement(drivers, chassis),
      chassisYaw(drivers),
      tracker(&chassisYaw, &chassisDisplacement){};

void OdometrySubsystem::initialize(){};
void OdometrySubsystem::refresh() { tracker.update(); };
};  // namespace odometry
}  // namespace subsystems