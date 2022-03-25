#include "flywheel_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace control
{
namespace flywheel
{
void FlywheelSubsystem::initialize() {}

void FlywheelSubsystem::refresh() {}

void FlywheelSubsystem::setDesiredOutput(float output)
{
    flywheelMotor.setTargetPwm(output);
}
}
}