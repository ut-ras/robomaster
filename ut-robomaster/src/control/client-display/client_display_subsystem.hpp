#pragma once

#include <vector>

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"
#include "tap/control/subsystem.hpp"

#include "control/client-display/graphics/graphic_abstract.hpp"
#include "control/client-display/graphics/graphic_circle.hpp"
#include "control/client-display/graphics/graphic_flywheel_on.hpp"
#include "control/client-display/graphics/graphic_orientation.hpp"
#include "control/client-display/graphics/graphic_reticle.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

using subsystems::flywheel::FlywheelSubsystem;
using subsystems::turret::TurretSubsystem;

namespace subsystems::control
{
class ClientDisplaySubsystem : public tap::control::Subsystem
{
public:
    ClientDisplaySubsystem(src::Drivers*, TurretSubsystem* turret, FlywheelSubsystem* flywheel);

    // add graphics desired here
    void initialize() override;

    void refresh() override;

    graphic::graphic_abstract* getCircle() { return &circle; };
    graphic::graphic_abstract* getReticle() { return &reticle; };
    graphic::graphic_abstract* getFlywheelOn() { return &flywheel_on; };
    graphic::graphic_abstract* getOrientation() { return &orientation; };

    void runHardwareTests() override;

    const char* getName() override { return "Client Display subsystem"; }

private:
    uint8_t* numGraphics;
    src::Drivers* drivers;
    TurretSubsystem* turret;
    FlywheelSubsystem* flywheel;

    graphic::graphic_circle circle{drivers};
    graphic::graphic_reticle reticle{drivers, turret};
    graphic::graphic_flywheel_on flywheel_on{drivers, flywheel};
    graphic::graphic_orientation orientation{drivers, turret};
};
}  // namespace subsystems::control