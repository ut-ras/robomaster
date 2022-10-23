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
    if (agitator.isNearTarget(targetAngle) && ballsToShoot > 0)
    {
        targetAngle += AGITATOR_INCREMENT_AMOUNT;
        ballsToShoot--;
    }
    agitator.rotateToTarget(targetAngle);

    // for fun    
    test.set(Leds::LedPin::Green, (targetAngle % 3) == 0);
    test.set(Leds::LedPin::Red, (targetAngle % 3) == 1);
    test.set(Leds::LedPin::Blue, (targetAngle % 3) == 2);
}

void ShooterSubsystem::shootBalls(int numBalls)
{
    ballsToShoot += numBalls;
}


void ShooterSubsystem::refresh() {}

}
}