#pragma once

// #include "tap/communication/gpio/pwm.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace startup
{
using tap::gpio::Pwm;

class StartupSubsystem : public tap::control::Subsystem
{
public:
    StartupSubsystem(src::Drivers* drivers);
    void initialize() override;
    void refresh() override;
    const char* getName() override { return "Startup subsystem"; }

    void setBuzzerFrequency(int frequency);
    void silence();

private:
    src::Drivers* drivers;
};
}  // namespace startup
}  // namespace subsystems