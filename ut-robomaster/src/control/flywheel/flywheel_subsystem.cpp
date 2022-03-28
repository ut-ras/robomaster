#include "flywheel_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace control
{
namespace flywheel
{
void FlywheelSubsystem::initialize() { flywheelMotor.setTargetPwm(1.0f); }

void FlywheelSubsystem::refresh() {}

void FlywheelSubsystem::setDesiredOutput(float output)
{
    flywheelMotor.updateSendPwmRamp();
}
}
}