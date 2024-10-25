#include "subsystem.hpp"

namespace subsystems
{
UTSubsystem::UTSubsystem(src::Drivers* drivers) : Subsystem(drivers) {}
bool UTSubsystem::hardwareOk() { return true; }

bool UTSubsystem::isAmputated() { return amputated; }

void UTSubsystem::setAmputated(bool newAmputatedValue) { amputated = newAmputatedValue; }
}  // namespace subsystems
