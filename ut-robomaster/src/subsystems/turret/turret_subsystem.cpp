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
      yawMotor(drivers, ID_YAW, CAN_TURRET, false, "yaw"),
      pitchMotor(drivers, ID_PITCH, CAN_TURRET, false, "pitch"),
      yawTurret(drivers, &yawMotor, YAW_PID_CONFIG),
      pitchTurret(drivers, &pitchMotor, PITCH_PID_CONFIG),
      turretOffset(0.0f, 0.0f, M_TWOPI)
{
}

void TurretSubsystem::initialize()
{
    yawTurret.initialize();
    pitchTurret.initialize();
    lastTime = tap::arch::clock::getTimeMilliseconds();
}

void TurretSubsystem::inputManualAngles(float yaw, float pitch)
{
    inputYaw = yaw;
    inputPitch = pitch;
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

void TurretSubsystem::setAimStrategy(AimStrategy aimStrategy) { this->aimStrategy = aimStrategy; }

float TurretSubsystem::getChassisYaw() { return modm::toRadian(drivers->bmi088.getYaw() - 180.0f); }
float TurretSubsystem::getTargetLocalYaw() { return targetWorldYaw - getChassisYaw(); }
float TurretSubsystem::getTargetLocalPitch() { return targetWorldPitch; }

float TurretSubsystem::getCurrentLocalYaw()
{
    return !isCalibrated ? 0.0f : yawTurret.getAngle() / BELT_RATIO - baseYaw;
}

float TurretSubsystem::getCurrentLocalPitch()
{
    return !isCalibrated ? 0.0f : pitchTurret.getAngle() - basePitch;
}

void TurretSubsystem::refresh()
{
    float bulletVelocity = 1.0f;
    float offset = 0.0f;
    uint8_t numBallisticIterations = 1;

    switch (aimStrategy)
    {
        case AimStrategy::Manual:
            setTargetWorldAngles(inputYaw, inputPitch);
            break;
        case AimStrategy::AutoAim:
        {
            updateAutoAim();
            break;
        }
        case AimStrategy::AimAssist:  // unimplemented
            break;
    }

    yawTurret.updateMotorAngle();
    pitchTurret.updateMotorAngle();

    if (!isCalibrated && yawMotor.isMotorOnline() && pitchMotor.isMotorOnline())
    {
        baseYaw = yawTurret.getAngle() / BELT_RATIO;
        basePitch = pitchTurret.getAngle() - PITCH_MIN;
        isCalibrated = true;
    }

    if (isCalibrated && !drivers->isKillSwitched())
    {
        uint32_t time = tap::arch::clock::getTimeMilliseconds();
        uint32_t dt = time - lastTime;
        lastTime = time;

        yawTurret.setAngle((baseYaw + getTargetLocalYaw()) * BELT_RATIO, dt);
        pitchTurret.setAngle(basePitch + getTargetLocalPitch(), dt);
    }
    else
    {
        yawTurret.reset();
        pitchTurret.reset();
    }
}

void TurretSubsystem::runHardwareTests()
{
    // TODO
}

void TurretSubsystem::updateAutoAim()
{
    // only run if the BeagleBone is online
    if (!drivers->beaglebone.isOnline()) return;

    // only run when there's new data
    if (lastTurretDataIndex == drivers->beaglebone.turretDataIndex) return;
    lastTurretDataIndex = drivers->beaglebone.turretDataIndex;

    // only run if we have a target
    TurretData data = drivers->beaglebone.getTurretData();
    if (!data.hasTarget) return;

    float cameraToPitch = 0.13555f;
    float nozzleToPitch = 0.18151f;
    float cameraToBarrels = 0.0427f;
    float cameraXOffset = -0.0335f;
    float bulletSpeed = 15.0f;
    int numIterations = 2;

    // Pitch axis relative (y/z flipped)
    Vector3f targetPos(
        data.xPos + cameraXOffset,
        data.zPos + cameraToPitch,
        data.yPos + cameraToBarrels);
    Vector3f targetVel(data.xVel, data.zVel, data.yVel);
    Vector3f targetAcc(data.xAcc, data.zAcc, data.yAcc);

    if (useBallistics)
    {
        // Rotate to world relative pitch
        float a = getCurrentLocalPitch();
        const float matData[9] = {1.0f, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a)};
        modm::Matrix3f rotMat(matData);
        targetPos = rotMat * targetPos;
        targetVel = rotMat * targetVel;
        targetAcc = rotMat * targetAcc;

        MeasuredKinematicState kinState{targetPos, targetVel, targetAcc};

        float turretPitch = 0.0f;
        float turretYaw = 0.0f;
        float travelTime = 0.0f;

        bool validBallistcs = findTargetProjectileIntersection(
            kinState,
            bulletSpeed,
            numIterations,
            &turretPitch,
            &turretYaw,
            &travelTime,
            -nozzleToPitch);

        float currentWorldYaw = getCurrentLocalYaw() + getChassisYaw();

        setTargetWorldAngles(currentWorldYaw + turretYaw, turretPitch);
    }
    else
    {
        float deltaYaw = -atan(targetPos.x / targetPos.y);  // yaw is opposite to camera X
        float deltaPitch = atan(targetPos.z / targetPos.y);
        float scale = 0.006f;
        float currentWorldYaw = getCurrentLocalYaw() + getChassisYaw();
        float currentWorldPitch = getCurrentLocalPitch();
        setTargetWorldAngles(
            targetWorldYaw + deltaYaw * scale,
            targetWorldPitch + deltaPitch * scale);
    }
}
}  // namespace turret
}  // namespace subsystems