#include "agitator_subsystem.hpp"
#include <algorithm>

namespace subsystems::shooter
{

AgitatorSubsystem::AgitatorSubsystem(tap::Drivers *drivers, tap::motor::MotorId motorId, tap::can::CanBus canBusMotors)
    :   Subsystem(drivers),
        motorId(motorId),
        canBusMotors(canBusMotors),
        motorOutput(0),
        pidController(22.0f, 0.5f, 0.0f, 5000.0f, MAX_CURRENT_OUTPUT),
        targetAnglePidController(0.375f, 0.025f, 1.0f, 5000.0f, MAX_CURRENT_OUTPUT / 4.0f), // TODO: Tune PID
        motor(
            drivers,
            motorId,
            canBusMotors,
            true,
            "agitator motor"
        )
{
    setMotorOutput(0);
}

void AgitatorSubsystem::initialize()
{
    motor.initialize();
}

// void AgitatorSubsystem::refresh() {}

void AgitatorSubsystem::setMotorOutput(int desiredRPM)
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

void AgitatorSubsystem::rotateToTarget(int64_t targetPosition)
{
    int64_t currentPosition = motor.getEncoderUnwrapped();

    
    float normalizedDifferenceAbsolute = std::min(std::abs(static_cast<int32_t>(targetPosition - currentPosition) / 2048.0f), 1.0f);
    int32_t signOfDifference = (targetPosition - currentPosition) / std::abs(targetPosition - currentPosition);

    // normalizedDifferenceAbsolute -= 0.2f; 

    // // theoretically, this will be a value between 0 and MAX_CURRENT_OUTPUT / 4.0f
    // int32_t error = static_cast<int32_t>(  std::pow(normalizedDifferenceAbsolute, 2) * MAX_CURRENT_OUTPUT / 6.0f );

     
    int32_t error = signOfDifference * normalizedDifferenceAbsolute * MAX_CURRENT_OUTPUT / 4.0f;
    
    // error += 256.0 * -1 * signOfDifference;
    
    targetAnglePidController.update(error);
    motor.setDesiredOutput(static_cast<int32_t>(targetAnglePidController.getValue()));
}


}
