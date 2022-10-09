#ifndef SHOOTER_SUBSYSTEM_HPP_
#define SHOOTER_SUBSYSTEM_HPP_

#include "tap/drivers.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/communication/gpio/pwm.hpp"
#include "tap/util_macros.hpp"

namespace subsystems
{
namespace shooter
{

class ShooterSubsystem : public tap::control::Subsystem
{
public:
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;

    ShooterSubsystem(tap::Drivers *drivers);

    ~ShooterSubsystem() = default;

    void initialize() override;

    void setFlywheelOutput(double normalizedOutput);
    void setAgitatorOutput(int desiredRPM);

    void refresh() override;

    void checkRegistered();

private:
    // static constexpr tap::motor::MotorId FLYWHEEL_MOTOR_ID = tap::motor::MOTOR1;
    // static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;
    // uint16_t flywheelMotorOutput;
    // modm::Pid<float> flywheelPidController;
    // tap::motor::DjiMotor flywheelMotor;


    static constexpr tap::gpio::Pwm::Pin FLYWHEEL_MOTOR_PIN = tap::gpio::Pwm::C1;
    static constexpr float MAX_SNAIL_OUTPUT = 0.50f;    // max pwm input value
    static constexpr float MIN_SNAIL_OUTPUT = 0.25f;    // min pwm input value

    static constexpr tap::motor::MotorId AGITATOR_MOTOR_ID = tap::motor::MOTOR1;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;
    uint16_t agitatorMotorOutput;
    modm::Pid<float> agitatorPidController;
    tap::motor::DjiMotor agitatorMotor;

    
    tap::Drivers *drivers;
};

}
}

#endif