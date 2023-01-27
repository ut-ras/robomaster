#include "flywheel_subsystem.hpp"

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
#include "tap/drivers.hpp"
#endif

namespace subsystems::shooter
{

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
FlywheelSubsystem::FlywheelSubsystem(tap::Drivers *drivers, tap::gpio::Pwm::Pin motorPin)
    :   Subsystem(drivers),
        motorPin(motorPin)
{
    setMotorOutput(0.0);
}
#else
FlywheelSubsystem::FlywheelSubsystem(tap::Drivers *drivers, tap::motor::MotorId motorId, tap::can::CanBus canBusMotors)
    :   Subsystem(drivers),
        motorId(motorId),
        canBusMotors(canBusMotors),
        motorOutput(0),
        pidController(0.5f, 0.0f, 0.0f, 5000.0f, MAX_CURRENT_OUTPUT),
        motor(
            drivers,
            motorId,
            canBusMotors,
            true,
            FlywheelSubsystem::getMotorName(motorId)
        )
{
    setMotorOutput(0);
}

constexpr const char *FlywheelSubsystem::getMotorName(tap::motor::MotorId motorId)
{
#   define CASE_DJI_MOTOR_NUMBER(number) \
    case tap::motor::MOTOR##number:      \
        return "flywheel motor: " #number;

    switch (motorId)
    {
    CASE_DJI_MOTOR_NUMBER(1);
    CASE_DJI_MOTOR_NUMBER(2);
    CASE_DJI_MOTOR_NUMBER(3);
    CASE_DJI_MOTOR_NUMBER(4);
    CASE_DJI_MOTOR_NUMBER(5);
    CASE_DJI_MOTOR_NUMBER(6);
    CASE_DJI_MOTOR_NUMBER(7);
    CASE_DJI_MOTOR_NUMBER(8);
    default:
        return "flywheel motor: Unexpected MotorId";
    }

#   undef CASE_DJI_MOTOR_NUMBER
}
#endif

void FlywheelSubsystem::initialize()
{
#ifdef FLYWHEELS_USE_SNAIL_MOTORS
    drivers->pwm.write(MIN_SNAIL_OUTPUT, motorPin);
#else
    motor.initialize();
#endif
}

// void FlywheelSubsystem::refresh() {}

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
void FlywheelSubsystem::setMotorOutput(float normalizedOutput) const
{
    // Map [0.0f, 1.0f] to [MIN_SNAIL_OUTPUT, MAX_SNAIL_OUTPUT]
    drivers->pwm.write(
        MIN_SNAIL_OUTPUT + (MAX_SNAIL_OUTPUT - MIN_SNAIL_OUTPUT) * normalizedOutput,
        motorPin
    );
}
#else
void FlywheelSubsystem::setMotorOutput(int desiredRPM)
{
    if (desiredRPM == 0)
    {
        motor.setDesiredOutput(0);
    }
    else
    {
        pidController.update(desiredRPM - motor.getShaftRPM());
        motor.setDesiredOutput(static_cast<int32_t>(pidController.getValue()));
    }
}
#endif

}
