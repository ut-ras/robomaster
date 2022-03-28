#ifndef FLYWHEEL_SUBSYSTEM_HPP_
#define FLYWHEEL_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/servo.hpp"
#include "tap/util_macros.hpp"

namespace control
{
namespace flywheel
{

class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(tap::Drivers *drivers)
        : tap::control::Subsystem(drivers),
          flywheelMotor(
              drivers,
              FLYWHEEL_MOTOR_PIN,
              MAX_SNAIL_OUTPUT,
              MIN_SNAIL_OUTPUT,
              1.0f)
    {
    }    

    ~FlywheelSubsystem() = default;

    void initialize() override;

    void setDesiredOutput(float output);

    void refresh() override;

    const tap::motor::Servo &getFlywheelMotor() const {return flywheelMotor;}

private:
    static constexpr tap::gpio::Pwm::Pin FLYWHEEL_MOTOR_PIN = tap::gpio::Pwm::C1;

    tap::motor::Servo flywheelMotor;

    static constexpr float MAX_SNAIL_OUTPUT = 1.0f;
    static constexpr float MIN_SNAIL_OUTPUT = 0.0f;
};
}
}

#endif