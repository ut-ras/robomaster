#include "shooter_subsystem.hpp"

#include "tap/communication/gpio/leds.hpp"
#include "tap/communication/serial/remote.hpp"

#include "robots/standard/robot_comms.hpp"
using namespace tap;
using namespace tap::gpio;

tap::gpio::Leds test;

namespace subsystems
{
namespace shooter
{

ShooterSubsystem::ShooterSubsystem(tap::Drivers* drivers)
    : tap::control::Subsystem(drivers),
#ifdef FLYWHEELS_USE_SNAIL_MOTORS
      flywheel(drivers, FLYWHEEL_MOTOR_PIN),
#else
      flywheel(drivers, FLYWHEEL_MOTOR_ID, CAN_BUS_MOTORS),
#endif
      agitator(drivers, AGITATOR_MOTOR_ID, CAN_BUS_MOTORS),
      targetAngle(0)
{
}

void ShooterSubsystem::registerSubsystems()
{
    drivers->commandScheduler.registerSubsystem(&flywheel);
    drivers->commandScheduler.registerSubsystem(&agitator);
}

void ShooterSubsystem::initialize()
{
    flywheel.initialize();
    agitator.initialize();
    test.init();
}

#ifdef FLYWHEELS_USE_SNAIL_MOTORS
void ShooterSubsystem::setFlywheelOutput(double normalizedOutput)  // between 0 and 1
{
    flywheel.setMotorOutput(normalizedOutput);
}
#else
void ShooterSubsystem::setFlywheelOutput(int desiredRPM) { flywheel.setMotorOutput(desiredRPM); }
#endif

void ShooterSubsystem::setAgitatorOutput(int desiredRPM) { agitator.setMotorOutput(desiredRPM); }

void ShooterSubsystem::rotateAgitatorToTarget()
{
    if (agitator.isNearTarget(targetAngle) && ballsToShoot > 0)
    {
        targetAngle += AGITATOR_INCREMENT_AMOUNT;
        ballsToShoot--;
    }
    agitator.rotateToTarget(targetAngle);

    int valueToDisplay = ballsToShoot;
    // for fun
    test.set(Leds::LedPin::Green, (valueToDisplay % 3) == 0);
    test.set(Leds::LedPin::Red, (valueToDisplay % 3) == 1);
    test.set(Leds::LedPin::Blue, (valueToDisplay % 3) == 2);

    // comms::RobotCommsSingleton::print("Balls to shoot: %d", valueToDisplay);
}

void ShooterSubsystem::shootBalls(int numBalls)
{
    const char* toPrint = "SHOOTING BALLS; BEFORE COMMAND: ballsToShoot = %d, numBalls = %d";

    // comms::RobotCommsSingleton::print(toPrint, ballsToShoot, numBalls);

    ballsToShoot += numBalls;

    const char* newPrint = "AFTER ballsToShoot = %d, numBalls = %d";

    // add new balls to shoot to toPrint
    // comms::RobotCommsSingleton::print(newPrint, ballsToShoot, numBalls);

    // targetAngle += AGITATOR_INCREMENT_AMOUNT;
    // ballsToShoot--;
}

void ShooterSubsystem::refresh() {}

}  // namespace shooter
}  // namespace subsystems