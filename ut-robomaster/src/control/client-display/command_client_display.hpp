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

class BeybladeIndicator : protected modm::Resumable<2>
{
private:
    const uint8_t *graphicName = (uint8_t *)"\x00\x00\x01";
    RefSerialTransmitter &refSerialTransmitter;
    RefSerialData::Tx::Graphic5Message msg;

public:
    BeybladeIndicator(RefSerialTransmitter refSerialTransmitter)
        : refSerialTransmitter(refSerialTransmitter)
    {
    }

    modm::ResumableResult<bool> sendInitialGraphics(src::Drivers *drivers);
    modm::ResumableResult<bool> update();

    void initialize();
};

namespace commands
{
class CommandClientDisplay : public Command, modm::pt::Protothread
{
private:
    src::Drivers *drivers;
    RefSerialTransmitter refSerialTransmitter;
    BeybladeIndicator beybladeIndicator;

public:
    CommandClientDisplay(src::Drivers *drivers, FlywheelSubsystem *flywheel)
        : Command(),
          drivers(drivers),
          refSerialTransmitter(drivers),
          beybladeIndicator(refSerialTransmitter)
    {
        addSubsystemRequirement(flywheel);
    }

    bool run();

    void initialize() override;
    void execute() override;
    void end(bool) override;
    bool isFinished() const override;
    const char *getName() const override { return "client display"; }
};
}  // namespace commands