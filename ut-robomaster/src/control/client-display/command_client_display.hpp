#pragma once

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/control/command.hpp"

#include "control/client-display/client_display_subsystem.hpp"
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
    CommandClientDisplay(src::Drivers *drivers, FlywheelSubsystem *flywheel)
        : Command(),
          drivers(drivers),
          refSerialTransmitter(drivers)
    {
        addSubsystemRequirement(flywheel);
    }

    bool run();

    void initialize() override;
    void execute() override;
    void end(bool) override;
    bool isFinished() const override;
    const char *getName() const override { return "client display"; }

private:
    src::Drivers *drivers;
    RefSerialTransmitter refSerialTransmitter;
    const uint8_t graphicId[3] = {0, 0, 1};  // 3 byte identifier for this graphic element
    RefSerialData::Tx::Graphic1Message msg;
};
}  // namespace commands