#include "agitator_subsystem.hpp"

namespace subsystems::shooter
{

AgitatorSubsystem::AgitatorSubsystem(tap::Drivers *drivers, tap::motor::MotorId motorId, tap::can::CanBus canBusMotors)
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

}
