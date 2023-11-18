#include "agitator_subsystem.hpp"

namespace subsystems::agitator
{

using tap::algorithms::compareFloatClose;
using tap::arch::clock::getTimeMilliseconds;

/**
 * AgitatorSubsystem class instantiation
 */
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      leftAgitator{
          drivers,
          M2006,
          ID_AGITATOR_L,
          CAN_SHOOTER,
          false,
          "agitator left",
          PID_AGITATOR},
      rightAgitator{
          drivers,
          M2006,
          ID_AGITATOR_R,
          CAN_SHOOTER,
          true,
          "agitator right",
          PID_AGITATOR}
{
}

#elif defined(TARGET_HERO)
AgitatorSubsystem::AgitatorSubsystem(src::Drivers *drivers)
    : Subsystem(drivers),
      drivers(drivers),
      agitator{drivers, M3508, ID_AGITATOR, CAN_SHOOTER, false, "agitator", PID_AGITATOR},
      feeder
{
    drivers, M2006, ID_FEEDER, CAN_SHOOTER, false, "feeder", PID_FEEDER
}

#endif

/**
 * AgitatorSubsystem initialization
 *
 * Contains all processes that need to be executed on start up
 */
void AgitatorSubsystem::initialize()
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    leftAgitator.initialize();
    rightAgitator.initialize();

#elif defined(TARGET_HERO)
    agitator.initialize();
    feeder.initialize();
#endif
}

/**
 * Called every clock cycle
 * Operations within refresh are performed on repeat throughout the robot's operation
 *
 */
void AgitatorSubsystem::refresh()
{
    float time = getTimeMilliseconds() / 1000.0f;  // MAY BREAK ON WRAPPING!
    bool killSwitch = drivers->isKillSwitched();

    // int frequency = drivers->refSerial.getRobotData().turret.firingFreq;

    // // get time since last bullet fired
    // int timeSinceFiring =
    //     drivers->refSerial.getRobotData().turret.lastReceivedLaunchingInfoTimestamp;

    // get positions of left and right agitators

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    leftAgitator.setActive(!killSwitch);
    rightAgitator.setActive(!killSwitch);

    leftAgitator.update(getShapedVelocity(time, 1.0f, 0.0f, ballsPerSecondLeft));
    rightAgitator.update(getShapedVelocity(time, 1.0f, 1.0f, ballsPerSecondRight));

#elif defined(TARGET_HERO)
    agitator.setActive(!killSwitch);
    feeder.setActive(!killSwitch);

    agitator.update(getShapedVelocity(time, 1.0f, 1.0f, ballsPerSecondHero));
    feeder.update(compareFloatClose(ballsPerSecondHero, 0.0f, 1E-6) ? 0.0f : FEEDER_SPEED);
#endif
}

/**
 * Returns the time-dependant velocity of the bullet(projectile) following a curved path?
 *
 */
float AgitatorSubsystem::getShapedVelocity(float time, float a, float phi, float ballsPerSecond)
{
    float t = time * ballsPerSecond;
    float maxVel = ballsPerSecond / BALLS_PER_REV;
    return ((1.0f - a) * cos((2.0f * t + phi) * M_PI) + 1.0f) * maxVel;
}

/**
 * Sets the firing rate for both barrels (number of balls per second fed into the barrels by the
 * agitators)
 *
 */
void AgitatorSubsystem::setBallsPerSecond(float bps)
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    ballsPerSecondLeft = bps;
    ballsPerSecondRight = bps;

#elif defined(TARGET_HERO)
    ballsPerSecondHero = bps;
#endif
}

/**
 * Sets the firing rate individual for each of the two barrels (number of balls per second fed into
 * each barrel by its respective agitator)
 *
 */
void AgitatorSubsystem::setBallsPerSecond(float bpsLeft, float bpsRight)
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    ballsPerSecondLeft = bpsLeft;
    ballsPerSecondRight = bpsRight;

#elif defined(TARGET_HERO)
    ballsPerSecondHero = bpsLeft;
#endif
}

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
float AgitatorSubsystem::getLeftPosition() { return leftAgitator.measurePosition(); }
float AgitatorSubsystem::getRightPosition() { return rightAgitator.measurePosition(); }

#elif defined(TARGET_HERO)
float AgitatorSubsystem::getPosition() { return Agitator.measurePosition(); }
#endif

}  // namespace subsystems::agitator
