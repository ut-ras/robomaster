#include "gimbal_move_command.hpp"
#include "tap/control/command.hpp"
#include "tap/drivers.hpp"
#include <math.h>

#include "gimbal_subsystem.hpp"

#define POSDEADZONE 0.2
#define NEGDEADZONE -0.2
using namespace tap::communication::serial;

namespace control
{
namespace gimbal
{
    GimbalMoveCommand::GimbalMoveCommand(tap::Drivers *drivers, GimbalSubsystem *sub)
        : drivers(drivers), 
          gimbal(sub)
    {
        addSubsystemRequirement(sub);
    }

void GimbalMoveCommand::initialize() {}

void GimbalMoveCommand::execute() 
{ 
    float x = drivers->remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL);
    float y = -(drivers->remote.getChannel(Remote::Channel::RIGHT_VERTICAL));

    if (fabs(x) > POSDEADZONE || fabs(y) > POSDEADZONE){
        gimbal->setDesiredOutput(x, y);
    } else{
        gimbal->setDesiredOutput(0, 0);
    }

    gimbal->setDesiredOutput(x, y);
}

void GimbalMoveCommand::end(bool) { gimbal->setDesiredOutput(0.0f, 0.0f); }

bool GimbalMoveCommand::isFinished() const { return false; }
}  // namespace gimbal
}  // namespace control
