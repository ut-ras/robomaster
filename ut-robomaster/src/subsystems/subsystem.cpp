#include "subsystem.hpp"

namespace subsystems
{
UTSubsystem::UTSubsystem(src::Drivers* drivers) : Subsystem(drivers) {}
UTSubsystem::hardwareOk() {
    return true;
}

UTSubsystem::isAmputated() {
    return amputated;
}

UTSubsystem::setAmputated(bool newAmputatedValue) {
    amputated = newAmputatedValue;
}
}  // namespace subsystems
