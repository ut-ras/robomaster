#include "chassis_drive_keyboard_command.hpp"
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
    ChassisDriveKeyboardCommand::ChassisDriveKeyboardCommand(tap::Drivers *drivers, ChassisSubsystem *sub)
        : drivers(drivers), 
          chassis(sub)
    {
        addSubsystemRequirement(sub);
    }

void ChassisDriveKeyboardCommand::initialize() {
    xRamp->setTarget(1.0f);
    xRamp->setValue(0.0f);
    xToggle = false;
    yRamp->setTarget(1.0f);
    yRamp->setValue(0.0f);
    yToggle = false;
    rRamp->setTarget(1.0f);
    rRamp->setValue(0.0f);
    rToggle = false;
}

void ChassisDriveKeyboardCommand::execute() 
{ 
    float x = drivers->remote.keyPressed(Remote::Key::W) - drivers->remote.keyPressed(Remote::Key::S);
    float y = drivers->remote.keyPressed(Remote::Key::D) - drivers->remote.keyPressed(Remote::Key::A);
    float r = drivers->remote.keyPressed(Remote::Key::E) - drivers->remote.keyPressed(Remote::Key::Q);

    if (x != 0.0f) {
        xRamp->update(0.002f);
        xToggle = true;
    }

    else if (xToggle) {
        xRamp->setValue(0.0f);
        xToggle = false;
    }

    if (y != 0.0f) {
        yRamp->update(0.002f);
        yToggle = true;
    }

    else if (yToggle) {
        yRamp->setValue(0.0f);
        yToggle = false;
    }

    if (r != 0.0f) {
        rRamp->update(0.002f);
        rToggle = true;
    }

    else if (rToggle) {
        rRamp->setValue(0.0f);
        rToggle = false;
    }

    chassis->setDesiredOutput(x > 0.0f ? xRamp->getValue() : -(xRamp->getValue()), 
                              y > 0.0f ? yRamp->getValue() : -(yRamp->getValue()), 
                              r > 0.0f ? rRamp->getValue() : -(rRamp->getValue()));
}

void ChassisDriveKeyboardCommand::end(bool) { chassis->setDesiredOutput(0.0f, 0.0f, 0.0f); }

bool ChassisDriveKeyboardCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace control
