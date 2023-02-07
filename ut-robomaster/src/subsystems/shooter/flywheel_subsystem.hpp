#ifndef SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_

// Remove to use DJI motors
#define FLYWHEELS_USE_SNAIL_MOTORS

#include "tap/control/subsystem.hpp"

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
#   include "tap/communication/gpio/pwm.hpp"
#else
#   include "modm/math/filter/pid.hpp"
#   include "tap/communication/can/can.hpp"
#   include "tap/motor/dji_motor.hpp"
#endif


namespace subsystems
{
namespace shooter
{

// Steve's flywheels use the Snail motor
// Eventually, the new flywheels will use DJI motors
class FlywheelSubsystem : public tap::control::Subsystem
{
public:
#ifdef FLYWHEELS_USE_SNAIL_MOTORS
    FlywheelSubsystem(tap::Drivers *drivers, tap::gpio::Pwm::Pin motorPin);
#else
    FlywheelSubsystem(tap::Drivers *drivers, tap::motor::MotorId motorId, tap::can::CanBus canBusMotors);
#endif

    ~FlywheelSubsystem() = default;

    void initialize() override;

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
    // normalizedOutput is between 0.0f and 1.0f
    void setMotorOutput(float normalizedOutput) const;
#else
    void setMotorOutput(int desiredRPM);
#endif

    // Default refresh for Subsystem is no-op
    // void refresh() override;

private:
#ifdef FLYWHEELS_USE_SNAIL_MOTORS
    const tap::gpio::Pwm::Pin motorPin;
    static constexpr float MAX_SNAIL_OUTPUT = 0.50f;    // max pwm input value
    static constexpr float MIN_SNAIL_OUTPUT = 0.25f;    // min pwn input value
#else
    const tap::motor::MotorId motorId;
    const tap::can::CanBus canBusMotors;
    uint16_t motorOutput; // Unused?
    modm::Pid<float> pidController; // Should the PID controller use int instead? Check kp
    tap::motor::DjiMotor motor;
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;

    static constexpr const char *getMotorName(tap::motor::MotorId motorId);
#endif
};

}
}

#endif // SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
