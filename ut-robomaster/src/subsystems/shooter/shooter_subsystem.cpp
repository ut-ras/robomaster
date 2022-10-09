#include "shooter_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace subsystems
{
namespace shooter
{
    
ShooterSubsystem::ShooterSubsystem(tap::Drivers *drivers)
    :   tap::control::Subsystem(drivers),
        agitatorPidController(0.5f, 0.0f, 0.0f, 5000.0f, 8000.0f),
        // flywheelMotor(
        //     drivers,
        //     FLYWHEEL_MOTOR_ID,
        //     CAN_BUS_MOTORS,
        //     true,
        //     "flywheel motor"),
        agitatorMotor(
            drivers,
            AGITATOR_MOTOR_ID,
            CAN_BUS_MOTORS,
            true,
            "agitator motor"),
        drivers(drivers)
        {
            setAgitatorOutput(0);
            setFlywheelOutput(0);
        }
        
void ShooterSubsystem::initialize() { 
    // flywheelMotor.initialize();
    
    // drivers->pwm.write(1.0f, UV_LIGHT_PIN);

    agitatorMotor.initialize();
    drivers->pwm.write(MIN_SNAIL_OUTPUT, FLYWHEEL_MOTOR_PIN); // NOT SPINNING
    
}

void ShooterSubsystem::refresh() {}

void ShooterSubsystem::setAgitatorOutput(int desiredRPM)
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

void ShooterSubsystem::setFlywheelOutput(double normalizedOutput) // between 0 and 1
{
    double toAdd = normalizedOutput / 4; // 0 to 0.25     
    drivers->pwm.write(MIN_SNAIL_OUTPUT + toAdd, FLYWHEEL_MOTOR_PIN);
    
}

}
}