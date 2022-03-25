#include "agitator_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace control
{
namespace agitator
{
void AgitatorSubsystem::initialize() { agitatorMotor.initialize(); }

void AgitatorSubsystem::refresh() {}

void AgitatorSubsystem::setDesiredOutput(float output)
{
    agitatorMotor.setDesiredOutput(output);
}
}
}