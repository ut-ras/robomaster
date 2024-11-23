#pragma once

#include <vector>

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"
#include "tap/control/subsystem.hpp"

#include "control/client-display/graphics/graphic_abstract.hpp"
#include "control/client-display/graphics/graphic_circle.hpp"
#include "control/client-display/graphics/graphic_reticle.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

using subsystems::turret::TurretSubsystem;

namespace subsystems::control
{
class ClientDisplaySubsystem : public tap::control::Subsystem
{
public:
    ClientDisplaySubsystem(src::Drivers*, TurretSubsystem* turret);

    // add graphics desired here
    void initialize() override;

    void refresh() override;

    // function to  get the vector of graphics
    //  std::vector<graphic::graphic_abstract*> getGraphics() {return currentGraphics;};
    graphic::graphic_abstract* getGraphics() { return &currentGraphics; };

    void runHardwareTests() override;

    const char* getName() override { return "Client Display subsystem"; }

private:
    // std::vector<graphic::graphic_abstract*> currentGraphics;
    graphic::graphic_abstract* graphics[10];
    uint8_t* numGraphics;
    src::Drivers* drivers;
    TurretSubsystem* turret;
    graphic::graphic_circle currentGraphics;
};
}  // namespace subsystems::control