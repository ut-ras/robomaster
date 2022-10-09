#include "shooter_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace subsystems
{
namespace shooter
{
    
ShooterSubsystem::ShooterSubsystem(tap::Drivers *drivers)
    :   tap::control::Subsystem(drivers),
        flywheelMotorOutput(0.0f),
        flywheelPidController(0.5f, 0.0f, 0.0f, 5000.0f, 8000.0f),
        flywheelMotor(
            drivers,
            FLYWHEEL_MOTOR_ID,
            CAN_BUS_MOTORS,
            true,
            "flywheel motor"),
        drivers(drivers)
        {
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

}
}