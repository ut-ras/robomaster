#pragma once

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/control/command.hpp"

#include "control/client-display/client_display_subsystem.hpp"
#include "modm/processing/protothread.hpp"
#include "modm/processing/resumable.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"

#include "drivers.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using subsystems::chassis::ChassisSubsystem;
using subsystems::control::ClientDisplaySubsystem;

class BeybladeIndicator : protected modm::Resumable<2>
{
private:
    const uint8_t *graphicName = (const uint8_t *)"\x00\x00\x01";
    RefSerialTransmitter &refSerialTransmitter;
    RefSerialData::Tx::Graphic5Message msg;

public:
    explicit BeybladeIndicator(RefSerialTransmitter &refSerialTransmitter)
        : refSerialTransmitter(refSerialTransmitter)
    {
    }

    modm::ResumableResult<void> sendInitialGraphics();
    modm::ResumableResult<void> update();

    void initialize();
};

class FlywheelIndicator : protected modm::Resumable<2>
{
private:
    const uint8_t *graphicName = (const uint8_t *)"\x00\x00\x02";
    RefSerialTransmitter &refSerialTransmitter;
    RefSerialData::Tx::Graphic5Message msg;

public:
    explicit FlywheelIndicator(RefSerialTransmitter &refSerialTransmitter)
        : refSerialTransmitter(refSerialTransmitter)
    {
    }

    void initialize();
    modm::ResumableResult<void> sendInitialGraphics();
    modm::ResumableResult<void> update();
};

namespace commands
{
class CommandClientDisplay : public Command, modm::pt::Protothread
{
private:
    src::Drivers *drivers;
    RefSerialTransmitter refSerialTransmitter;
    BeybladeIndicator beybladeIndicator;
    FlywheelIndicator flywheelIndicator;
    bool restarting = true;

    void restartHud();

public:
    CommandClientDisplay(src::Drivers *drivers, ChassisSubsystem *chassis)
        : Command(),
          drivers(drivers),
          refSerialTransmitter(drivers),
          beybladeIndicator(refSerialTransmitter),
          flywheelIndicator(refSerialTransmitter)
    {
        addSubsystemRequirement(chassis);
    }

    bool run();

    const char *getName() const override { return "client display"; }
    void initialize() override
    {
        tap::buzzer::playNote(&drivers->pwm, 880);
        this->restarting = true;
    };

    void execute() override { run(); }
    void end(bool) override {}
    bool isFinished() const override { return false; }
};
}  // namespace commands
