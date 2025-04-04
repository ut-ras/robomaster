#pragma once

#include <vector>

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/control/command.hpp"

#include "control/client-display/client_display_subsystem.hpp"
#include "control/client-display/graphics/graphic_abstract.hpp"
#include "modm/processing/protothread.hpp"
#include "modm/processing/resumable.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"

#include "drivers.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using subsystems::control::ClientDisplaySubsystem;
using subsystems::flywheel::FlywheelSubsystem;

namespace commands
{
class CommandClientDisplay : public Command, modm::pt::Protothread
{
public:
    CommandClientDisplay(src::Drivers* drivers, ClientDisplaySubsystem* client)
        : Command(),
          drivers(drivers),
          client(client)
    {
        addSubsystemRequirement(client);
    }

    bool run();

    void initialize() override;
    void execute() override;
    void end(bool) override;
    bool isFinished() const override;
    const char* getName() const override { return "client display"; }

private:
    src::Drivers* drivers;
    ClientDisplaySubsystem* client;
    graphic::graphic_abstract* circle;
    graphic::graphic_abstract* reticle;
    graphic::graphic_abstract* flywheel_on;
    graphic::graphic_abstract* orientation;
    tap::arch::PeriodicMilliTimer hudTimer{50};
};
}  // namespace commands