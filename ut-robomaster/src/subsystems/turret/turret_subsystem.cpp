#include "turret_subsystem.hpp"

#include "tap/algorithms/ballistics.hpp"

#include "robots/robot_constants.hpp"

#include "modm/math.hpp"

using namespace tap::algorithms::ballistics;
namespace subsystems
{
namespace turret
{
TurretSubsystem::TurretSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      yawMotor(
          drivers,
          ID_YAW,
          CAN_TURRET,
          false,
          "Yaw Motor"),
      pitchMotor(
          drivers,
          ID_PITCH,
          CAN_TURRET,
          false,
          "Pitch Motor"),
      yawTurret(
          drivers,
          &yawMotor,
          YAW_PID_CONFIG
      ),
      pitchTurret(
          drivers,
          &pitchMotor,
          PITCH_PID_CONFIG
      )
{
}

void TurretSubsystem::initialize()
{
    yawTurret.initialize();
    pitchTurret.initialize();
    initialChassisYaw = drivers->bmi088.getYaw()*(M_PI/180.0);
    previousChassisYaw = initialChassisYaw;
}

void TurretSubsystem::setDesiredAngles(float yaw, float pitch)
{
    desiredYaw = yaw;
    desiredPitch = pitch;
}

void TurretSubsystem::inputTargetData(Vector3f position, Vector3f velocity, Vector3f acceleration)
{
    targetPosition = position;
    targetVelocity = velocity;
    targetAcceleration = acceleration;
}

void TurretSubsystem::setAimStrategy(AimStrategy aimStrategy) { this->aimStrategy = aimStrategy; }

void TurretSubsystem::calculateOffset() {
    float currentChassisYaw = drivers->bmi088.getYaw()*(M_PI/180.0);

    if(currentChassisYaw == previousChassisYaw) {
        offset.setValue(0.0f);
    } else {
        offset.setValue(initialChassisYaw-currentChassisYaw);
    }
    previousChassisYaw = currentChassisYaw;

   // offset=initialChassisYaw - currentChassisYaw;

    //offset = previousTurretYaw;
    // Mod just in case

    

    // Update values
   // previousTurretYaw = offset;

    //initialChassisYaw = currentChassisYaw;

    // previousYaw has prev. world-view yaw of turret
   // float currentYaw = getYawTurret()->getAngle() + (drivers->bmi088.getYaw()*(M_PI/180.0));  // chassis + world relative view
   // offset = previousYaw - currentYaw;

    //previousYaw = currentYaw;
    
    // offset = fmod(offset, 2*M_PI);
    // offset = offset-currentYaw;

    //offset the world relative angle
   // offset *= M_PI/180;
   // return offset;
}

void TurretSubsystem::refresh()
{
    float yaw = 0.0f;
    float pitch = 0.0f;
    float bulletVelocity = 1.0f;
    float offset = 0.0f;
    uint8_t numBallisticIterations = 1;

    switch (aimStrategy)
    {
        case AimStrategy::Manual:
            yaw = desiredPitch;
            pitch = desiredPitch;
            break;
        case AimStrategy::AutoAim:
        {
            float turretPitch = 0.0f;
            float turretYaw = 0.0f;
            float projectedTravelTime = 0.0f;

            findTargetProjectileIntersection(
                {targetPosition, targetVelocity, targetAcceleration},
                bulletVelocity,
                numBallisticIterations,
                &turretPitch,
                &turretYaw,
                &projectedTravelTime);

            yaw = turretYaw;
            pitch = turretPitch;
            break;
        }
        case AimStrategy::AimAssist:  // unimplemented
            break;
    }

    calculateOffset();

    yawTurret.updateMotorAngle();
    pitchTurret.updateMotorAngle();
}


void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems