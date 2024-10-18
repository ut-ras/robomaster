#include "turret_subsystem.hpp"

#include "tap/algorithms/ballistics.hpp"

#include "modm/math.hpp"
#include "robots/robot_constants.hpp"

namespace subsystems
{
namespace turret
{
using namespace tap::algorithms::ballistics;
using communication::TurretData;
using modm::Vector2f;

TurretSubsystem::TurretSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
#if defined(TARGET_STANDARD) || defined(TARGET_HERO)
      yawEncoder(),
      yaw(drivers, YAW_L, YAW_R, &yawEncoder),
#else
      yaw(drivers, YAW, YAW_PID_CONFIG),
#endif
      pitch(drivers, PITCH, PITCH_PID_CONFIG),
      turretOffset(0.0f, 0.0f, M_TWOPI)
{
}

void TurretSubsystem::initialize()
{
    yaw.initialize();
    pitch.initialize();
}

void TurretSubsystem::refresh()
{
#if defined(TARGET_STANDARD) || defined(TARGET_HERO)
    yawEncoder.update();
#endif

    yaw.updateMotorAngle();
    pitch.updateMotorAngle();

    if (!isCalibrated && yaw.isOnline() && pitch.isOnline())
    {
        baseYaw = yaw.getAngle() / YAW_REDUCTION;
        basePitch = pitch.getAngle() / PITCH_REDUCTION - PITCH_MIN;
        isCalibrated = true;
    }

    if (isCalibrated && !drivers->isKillSwitched())
    {
        yaw.setAngle((baseYaw + getTargetLocalYaw()) * YAW_REDUCTION, DT);
        pitch.setAngle((basePitch + getTargetLocalPitch()) * PITCH_REDUCTION, DT);
    }
    else
    {
        yaw.reset();
        pitch.reset();
    }
}

void TurretSubsystem::inputTargetData(Vector3f position, Vector3f velocity, Vector3f acceleration)
{
    targetPosition = position;
    targetVelocity = velocity;
    targetAcceleration = acceleration;
}

void TurretSubsystem::setTargetWorldAngles(float yaw, float pitch)
{
    targetWorldYaw = yaw;
    targetWorldPitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);
}

float TurretSubsystem::getChassisYaw() { return modm::toRadian(drivers->bmi088.getYaw() - 180.0f); }

float TurretSubsystem::getTargetLocalYaw() { return targetWorldYaw - getChassisYaw(); }

float TurretSubsystem::getTargetLocalPitch() { return targetWorldPitch; }

float TurretSubsystem::getCurrentLocalYaw()
{
    return !isCalibrated ? 0.0f : yaw.getAngle() / YAW_REDUCTION - baseYaw;
}

float TurretSubsystem::getCurrentLocalPitch()
{
    return !isCalibrated ? 0.0f : pitch.getAngle() / PITCH_REDUCTION - basePitch;
}

float TurretSubsystem::getBulletDropReticle(){
    /* bullet drop based on target position
    Vector3f targetPos(
        data.xPos + CAMERA_X_OFFSET,
        data.zPos + CAMERA_TO_PITCH,
        data.yPos + CAMERA_TO_BARRELS);
    Vector3f targetVel(data.xVel, data.zVel, data.yVel);
    Vector3f targetAcc(data.xAcc, data.zAcc, data.yAcc);

    // Rotate to world relative pitch
        float a = turret->getCurrentLocalPitch();
        const float matData[9] = {1.0f, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a)};
        modm::Matrix3f rotMat(matData);
        targetPos = rotMat * targetPos;
        targetVel = rotMat * targetVel;
        targetAcc = rotMat * targetAcc;

        MeasuredKinematicState kinState{targetPos, targetVel, targetAcc};

        float turretPitch = 0.0f;
        float turretYaw = 0.0f;
        float travelTime = 0.0f;

        bool projectileTime = computeTravelTime(
            kinState,
            TARGET_PROJECTILE_VELOCITY,
            BALLISTIC_ITERATIONS,
            &turretPitch,
            &turretYaw,
            &travelTime,
            -NOZZLE_TO_PITCH);
        */

        // physical distance  = 0.5*9.8*powf(travelTime, 2)
        // angle from turret = atan(hdrop/d)
        float travelTime = distance/TARGET_PROJECTILE_VELOCITY;
        float dropDistance = 0.5*9.8*powf(travelTime, 2);
        // float targetDistance = hypot(targetPos.x, targetPos.y) - NOZZLE_TO_PITCH;
        float reticleDistance = atan(dropDistance/distance);
        // linear interpolation. simple, but it is less effective the more distorted the camera is
        // 80, guess for vertical angle range. 720, number of pixels in vertical direction
        float cameraAnglePixelScale = 80/720; // idk what the actual camera fov is, software didn't know and we can change it 
        
        //number of pixels below current camera position
        return reticleDistance*cameraAnglePixelScale+NOZZLE_TO_PITCH; // target pitch - current pitch = bullet drop over distance?
        // } // i'm assuming that the camera is pointed above the turret

        // return 0;
}


bool TurretSubsystem::getIsCalibrated() { return isCalibrated; }

void TurretSubsystem::runHardwareTests()
{
    // TODO
}
}  // namespace turret
}  // namespace subsystems