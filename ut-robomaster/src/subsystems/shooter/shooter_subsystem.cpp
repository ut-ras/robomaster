#include "shooter_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

using namespace tap;

namespace subsystems
{
namespace shooter
{
    
ShooterSubsystem::ShooterSubsystem(tap::Drivers *drivers)
    :   tap::control::Subsystem(drivers),
#ifdef FLYWHEELS_USE_SNAIL_MOTORS
        flywheel(drivers, FLYWHEEL_MOTOR_PIN),
#else
        flywheel(drivers, FLYWHEEL_MOTOR_ID, CAN_BUS_MOTORS),
#endif
        agitator(drivers, AGITATOR_MOTOR_ID, CAN_BUS_MOTORS)
{}

void ShooterSubsystem::registerSubsystems()
{
    drivers->commandScheduler.registerSubsystem(&flywheel);
    drivers->commandScheduler.registerSubsystem(&agitator);
}
        
void ShooterSubsystem::initialize() { 
    flywheel.initialize();
    agitator.initialize();
}

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
void ShooterSubsystem::setFlywheelOutput(double normalizedOutput) // between 0 and 1
{
    flywheel.setMotorOutput(normalizedOutput);
}
#else
void ShooterSubsystem::setFlywheelOutput(int desiredRPM)
{
    flywheel.setMotorOutput(desiredRPM);
}
#endif

void ShooterSubsystem::setAgitatorOutput(int desiredRPM)
{
    agitator.setMotorOutput(desiredRPM);
}

void ShooterSubsystem::refresh() {}

}
}