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

bool TurretSubsystem::getIsCalibrated() { return isCalibrated; }

void TurretSubsystem::runHardwareTests()
{
    // TODO
}
}  // namespace turret
}  // namespace subsystems