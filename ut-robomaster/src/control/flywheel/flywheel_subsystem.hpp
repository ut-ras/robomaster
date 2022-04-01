#ifndef FLYWHEEL_SUBSYSTEM_HPP_
#define FLYWHEEL_SUBSYSTEM_HPP_

#include "tap/drivers.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/communication/gpio/pwm.hpp"
#include "tap/util_macros.hpp"

namespace control
{
namespace flywheel
{

class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(tap::Drivers *drivers);
        
    ~FlywheelSubsystem() = default;

    void initialize() override;

    void setDesiredOutput(float output);

    void refresh() override;

private:
    static constexpr tap::gpio::Pwm::Pin FLYWHEEL_MOTOR_PIN = tap::gpio::Pwm::C1;
    static constexpr float MAX_SNAIL_OUTPUT = 1.0f;
    static constexpr float MIN_SNAIL_OUTPUT = 0.0f;

    tap::Drivers *drivers;
};
}
}

#endif