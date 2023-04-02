#include "turret_subsystem.hpp"

#include "tap/algorithms/ballistics.hpp"

#include "robots/robot_constants.hpp"

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

/*
* Ver 1)
*   Finding offset (Current - Prev)
*   float lastUpdatedYawValue = 0.0f;   // put this global somewhere priate in turret motor or subsystem
*   the yaw of the turret is chassis relative
*   return the negative offset
* Ver 2)
*   Finding offset based off of worldview (current values only)
*   Subtracting the yaw value from the chassis from the turret yaw ?
*   -yaw + turret value
*/

float TurretSubsystem::calculateOffset() {
    float offset = 0.0f;
    // if(drivers->bmi088.getYaw() > ) {
        
    // }
    // tap::algorithms::ContiguousFloat currentAngle;

    float yaw = -1*drivers->bmi088.getYaw();   // return radians
    //float turretYaw = subsystem->getYawTurret()->getAngle();

            //offset = -getYaw; -> 
        //offset%=360;  // Make sure if the range is (0,180) to (-180,0)
            //if(yaw>180)
                // offset = -(360-getYaw);
            //convert to radians
            // use this function for conversion modm_toradians

    return offset;
        // Pseudo code implementation:
    // create a global variable for offset and initialize to zero
    // get the yaw value from imu in degrees
    // encoder in radians
    // every time yaw value gets updated store a new offset
        //offset = -getYaw; -> 
        //offset%=360;  // Make sure if the range is (0,180) to (-180,0)
            //if(yaw>180)
                // offset = -(360-getYaw);
            //convert to radians
            // use this function for conversion modm_toradians
    // edge case:
    // consider the wrapper function, contiguous float? to get smallest degree
    // manual implementation: check 180 and subtract degrees from 360 (make negative for opposite)
    float yawSetpoint = getYawTurret()->getAngle();
    
    drivers->bmi088.getImuState();  // state of IMU (Turned on? Calibrated?) -- enum states
    drivers->bmi088.getYaw();   // return inradians
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

    yawTurret.updateMotorAngle();
    pitchTurret.updateMotorAngle();
}


void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems