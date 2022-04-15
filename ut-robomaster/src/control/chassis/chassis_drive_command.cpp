#include "chassis_drive_command.hpp"
#include "tap/control/command.hpp"
#include "tap/drivers.hpp"
#include <math.h>

#include "chassis_subsystem.hpp"

#define POSDEADZONE 0.2
#define NEGDEADZONE -0.2
using namespace tap::communication::serial;

namespace control
{
namespace chassis
{
    ChassisDriveCommand::ChassisDriveCommand(tap::Drivers *drivers, ChassisSubsystem *sub)
        : drivers(drivers), 
          chassis(sub)
    {
        addSubsystemRequirement(sub);
    }

void ChassisDriveCommand::initialize() {}

void ChassisDriveCommand::execute() 
{ 
    float x = drivers->remote.getChannel(Remote::Channel::LEFT_HORIZONTAL);
    float y = -(drivers->remote.getChannel(Remote::Channel::LEFT_VERTICAL));
    float r = drivers->remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL);

    if (fabs(x) > POSDEADZONE || fabs(y) > POSDEADZONE || fabs(r) > POSDEADZONE){
        chassis->setDesiredOutput(x, y, r);
    } else{
        chassis->setDesiredOutput(0, 0, 0);
    }

    chassis->setDesiredOutput(x, y, r);
}

void ChassisDriveCommand::end(bool) { chassis->setDesiredOutput(0.0f, 0.0f, 0.0f); }

bool ChassisDriveCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace control
