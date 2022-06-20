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
        agitatorPidController(0.5f, 0.0f, 0.0f, 5000.0f, 8000.0f),
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
        agitatorPidController.update(desiredRPM - agitatorMotor.getShaftRPM());
        agitatorMotor.setDesiredOutput(static_cast<int32_t> (agitatorPidController.getValue()));
    }
}

}
}