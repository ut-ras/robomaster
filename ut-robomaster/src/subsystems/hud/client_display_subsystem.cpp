#include "client_display_subsystem.hpp"

#include "tap/communication/sensors/buzzer/buzzer.hpp"

namespace subsystems::hud
{
ClientDisplaySubsystem::ClientDisplaySubsystem(
    src::Drivers* drivers,
    TurretSubsystem* turret,
    FlywheelSubsystem* flywheel)
    : Subsystem(drivers),
      drivers(drivers),
      turret(turret),
      flywheel(flywheel),
      circle(drivers),
      reticle(drivers, ClientDisplaySubsystem::turret),
      flywheel_on(drivers, ClientDisplaySubsystem::flywheel),
      orientation(drivers, ClientDisplaySubsystem::turret)

{
}

void ClientDisplaySubsystem::initialize() { *numGraphics = 4; }

void ClientDisplaySubsystem::refresh() {}

void ClientDisplaySubsystem::runHardwareTests() {}

}  // namespace subsystems::hud