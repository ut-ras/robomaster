#include "chassis_drive_command.hpp"
#include "tap/control/command.hpp"
#include "tap/drivers.hpp"

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
    float x = drivers->remote.getChannel(Remote::Channel::LEFT_HORIZONTAL) * 1.1;
    float y = -(drivers->remote.getChannel(Remote::Channel::LEFT_VERTICAL));
    float r = drivers->remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL);

    if (x > POSDEADZONE || y > POSDEADZONE || r > POSDEADZONE || x < NEGDEADZONE || y < NEGDEADZONE || r < NEGDEADZONE){
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
