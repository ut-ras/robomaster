#include "agitator_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

#include "tap/drivers.hpp"

using namespace tap;

namespace control
{
namespace agitator
{
AgitatorSubsystem::AgitatorSubsystem(tap::Drivers* drivers)
    :   tap::control::Subsystem(drivers),
        agitatorMotorOutput(0.0f),
        agitatorPidController(100, 0, 0, 0, 8000, 1, 0, 1, 0),
        agitatorMotor(
            drivers,
            AGITATOR_MOTOR_ID,
            CAN_BUS_MOTORS,
            false,
            "agitator motor"),
        drivers(drivers)
    {
    }

void AgitatorSubsystem::initialize() { 
    
    agitatorMotor.initialize();
}

void AgitatorSubsystem::refresh() {}

void AgitatorSubsystem::setDesiredRPM(int desiredRPM)
{
    if (desiredRPM == 0)
    {
        agitatorMotor.setDesiredOutput(0.0f);
    }

    else
    {
        agitatorPidController.runControllerDerivateError((desiredRPM - agitatorMotor.getShaftRPM()), 1);
        agitatorMotor.setDesiredOutput(static_cast<int32_t> (agitatorPidController.getOutput()));
    }
}

}
}