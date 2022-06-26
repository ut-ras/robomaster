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
    xRamp.setTarget(1.0f);
    xRamp.setValue(0.0f);
    xToggle = false;
    xPositive = true;
    xDeacceleration = false;
    yRamp.setTarget(1.0f);
    yRamp.setValue(0.0f);
    yToggle = false;
    yPositive = true;
    yDeacceleration = false;
    rRamp.setTarget(1.0f);
    rRamp.setValue(0.0f);
    rToggle = false;
    rPositive = true;
    rDeacceleration = false;

    isBeyblade = false;
}

void ChassisDriveKeyboardCommand::execute() 
{ 
    float x = drivers->remote.keyPressed(Remote::Key::D) - drivers->remote.keyPressed(Remote::Key::A);
    float y = -(drivers->remote.keyPressed(Remote::Key::W) - drivers->remote.keyPressed(Remote::Key::S));
    float r = -(drivers->remote.keyPressed(Remote::Key::Q) - drivers->remote.keyPressed(Remote::Key::E));

    if (drivers->remote.keyPressed(Remote::Key::R)) {
        if (!isBeyblade) {
            r = 1.0f;
            isBeyblade = true;
        }
        else {
            r = 0.0f;
            isBeyblade = false;
        }
    }

    if (x != 0.0f && !xDeacceleration) {
        if (xRamp.getTarget() != 1.0f) {
            xRamp.setTarget(1.0f);
        }

        xRamp.update(0.002f);
        xToggle = true;

        if (x > 0.0f) { xPositive = true; }
        else { xPositive = false; }
    }

    else if (xToggle) {
        xDeacceleration = true;
        if (xRamp.getTarget() != 0.0f) {
            xRamp.setTarget(0.0f);
        }

        xRamp.update(0.004f);
        if (xRamp.getValue() == 0.0f) {
            xToggle = false;
            xDeacceleration = false;
        }
    }

    if (y != 0.0f && !yDeacceleration) {
        if (yRamp.getTarget() != 1.0f) {
            yRamp.setTarget(1.0f);
        }

        yRamp.update(0.002f);
        yToggle = true;

        if (y > 0.0f) { yPositive = true; }
        else { yPositive = false; }
    }

    else if (yToggle) {
        yDeacceleration = true;
        if (yRamp.getTarget() != 0.0f) {
            yRamp.setTarget(0.0f);
        }

        yRamp.update(0.004f);
        if (yRamp.getValue() == 0.0f) {
            yToggle = false;
            yDeacceleration = false;
        }
    }

    if (r != 0.0f && !rDeacceleration) {
        if (rRamp.getTarget() != 1.0f) {
            rRamp.setTarget(1.0f);
        }

        rRamp.update(0.002f);
        rToggle = true;

        if (r > 0.0f) { rPositive = true; }
        else { rPositive = false; }
    }

    else if (rToggle) {
        rDeacceleration = true;
        if (rRamp.getTarget() != 0.0f) {
            rRamp.setTarget(0.0f);
        }

        rRamp.update(0.004f);
        if (rRamp.getValue() == 0.0f) {
            rToggle = false;
            rDeacceleration = false;
        }
    }

    chassis->setDesiredOutput(xPositive ? xRamp.getValue() : -(xRamp.getValue()), 
                              yPositive ? yRamp.getValue() : -(yRamp.getValue()), 
                              rPositive ? rRamp.getValue() : -(rRamp.getValue()));
}

void ChassisDriveKeyboardCommand::end(bool) { chassis->setDesiredOutput(0.0f, 0.0f, 0.0f); }

bool ChassisDriveKeyboardCommand::isFinished() const { return false; }
}  // namespace chassis
}  // namespace control
