#include "subsystem.hpp"

namespace subsystems
{
Subsystem::Subsystem(src::Drivers* drivers) : tap::control::Subsystem(drivers) {}
bool Subsystem::hardwareOk() { return true; }

bool Subsystem::isAmputated() { return amputated; }

void Subsystem::setAmputated(bool newAmputatedValue) { amputated = newAmputatedValue; }
}  // namespace subsystems
