#include "client_display_subsystem.hpp"

#include "tap/communication/sensors/buzzer/buzzer.hpp"

namespace subsystems::control
{
ClientDisplaySubsystem::ClientDisplaySubsystem(src::Drivers* drivers, TurretSubsystem* turret)
    : Subsystem(drivers),
      drivers(drivers),
      turret(turret),
      currentGraphics(drivers)
{
}

void ClientDisplaySubsystem::initialize()
{
    // graphic::graphic_circle circle2(drivers);
    // graphic::graphic_circle* circle = new graphic::graphic_circle(drivers);
    // graphic::graphic_reticle* reticle = new graphic::graphic_reticle(drivers, turret);
    // currentGraphics.emplace_back(circle);
    // currentGraphics = circle2;
    // currentGraphics = reticle;
    *numGraphics = 2;
}

void ClientDisplaySubsystem::refresh() {}

void ClientDisplaySubsystem::runHardwareTests() {}

}  // namespace subsystems::control