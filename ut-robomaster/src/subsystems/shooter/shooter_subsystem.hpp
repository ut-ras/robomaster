#ifndef SHOOTER_SUBSYSTEM_HPP_
#define SHOOTER_SUBSYSTEM_HPP_

#include "agitator_subsystem.hpp"
#include "flywheel_subsystem.hpp"

#include "tap/drivers.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/communication/gpio/pwm.hpp"
#include "tap/util_macros.hpp"

namespace subsystems
{
namespace shooter
{

// Manages Flywheel and Agitator subsystems
class ShooterSubsystem : public tap::control::Subsystem
{
public:
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;

    ShooterSubsystem(tap::Drivers *drivers);

    ~ShooterSubsystem() = default;

    // Registers the Flywheel and Agitator subsystems
    void registerSubsystems();

    void initialize() override;

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
    // Float or double?
    void setFlywheelOutput(double normalizedOutput);
#else
    void setFlywheelOutput(int desiredRPM);
#endif
    void setAgitatorOutput(int desiredRPM);

    // Subsystem is no-op, unnecessary override?
    void refresh() override;

    // Unused?
    void checkRegistered();

private:
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
    static constexpr tap::gpio::Pwm::Pin FLYWHEEL_MOTOR_PIN = tap::gpio::Pwm::C1;
#else
    static constexpr tap::motor::MotorId FLYWHEEL_MOTOR_ID = tap::motor::MOTOR1;
#endif

    static constexpr tap::motor::MotorId AGITATOR_MOTOR_ID = tap::motor::MOTOR1;

    FlywheelSubsystem flywheel;
    AgitatorSubsystem agitator;
};

}
}

#endif