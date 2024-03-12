#pragma once

#include "tap/communication/gpio/pwm.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace sound
{
using tap::gpio::Pwm;

class SoundSubsystem : public tap::control::Subsystem
{
public:
    SoundSubsystem(src::Drivers* drivers);
    void initialize() override;
    void refresh() override;
    const char* getName() override { return "Sound subsystem"; }

    void setBuzzerFrequency(int frequency);
    void silence();

private:
    src::Drivers* drivers;
};
}  // namespace sound
}  // namespace subsystems