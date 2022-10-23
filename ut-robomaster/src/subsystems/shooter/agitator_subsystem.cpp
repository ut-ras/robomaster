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
        targetAnglePidController(0.75f, 0.00f, 75.0f, 100000.0f, MAX_CURRENT_OUTPUT / 4), // TODO: Tune PID
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

    int32_t error = signOfDifference * normalizedDifferenceAbsolute * MAX_CURRENT_OUTPUT / 4.0f;
    
    targetAnglePidController.update(targetPosition - currentPosition);
    motor.setDesiredOutput(static_cast<int32_t>(targetAnglePidController.getValue()));
}

bool AgitatorSubsystem::isNearTarget(int64_t targetPosition)
{
    int64_t currentPosition = motor.getEncoderUnwrapped();
    return std::abs(targetPosition - currentPosition) < NEAR_TARGET_THRESHOLD;
}

}
