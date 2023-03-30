#include "shooter_single_fire_command.hpp"

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command.hpp"

#include "robots/standard/robot_comms.hpp"

#include "shooter_subsystem.hpp"

namespace subsystems
{
namespace shooter
{
void ShooterSingleFireCommand::initialize() {}

void ShooterSingleFireCommand::execute()
{
    // comms::RobotCommsSingleton::print("ShooterSingleFireCommand::execute()");

    shooter->shootBalls(2);
}

void ShooterSingleFireCommand::end(bool) {}

bool ShooterSingleFireCommand::isFinished() const { return false; }
}  // namespace shooter
}  // namespace subsystems