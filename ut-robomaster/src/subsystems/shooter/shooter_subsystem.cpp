#include "shooter_subsystem.hpp"
#include "tap/communication/gpio/leds.hpp"
#include "tap/communication/serial/remote.hpp"

using namespace tap;
using namespace tap::gpio;

tap::gpio::Leds test;

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
        agitator(drivers, AGITATOR_MOTOR_ID, CAN_BUS_MOTORS),
        targetAngle(0)
{}

void ShooterSubsystem::registerSubsystems()
{
    drivers->commandScheduler.registerSubsystem(&flywheel);
    drivers->commandScheduler.registerSubsystem(&agitator);
}
        
void ShooterSubsystem::initialize() { 
    flywheel.initialize();
    agitator.initialize();
    test.init();
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

void ShooterSubsystem::rotateAgitatorToTarget()
{
    int x = targetAngle;
    test.set(Leds::LedPin::Green, (x % 3) == 0);
    test.set(Leds::LedPin::Red, (x % 3) == 1);
    test.set(Leds::LedPin::Blue, (x % 3) == 2);
    agitator.rotateToTarget(targetAngle);
}

void ShooterSubsystem::incrementAgitatorTargetAngle(uint64_t increment)
{
    targetAngle = targetAngle + 2820;
    //rotateAgitatorToTarget();
    
}


void ShooterSubsystem::refresh() {}

}
}