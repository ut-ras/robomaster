#include "turret_subsystem.hpp"

#include "tap/communication/serial/remote.hpp"

#include "modm/math.hpp"
#include "robots/robot_constants.hpp"
#include "subsystems/subsystem.hpp"

namespace subsystems::turret
{
using tap::communication::serial::Remote;
TurretSubsystem::TurretSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
#if defined(TARGET_STANDARD) || defined(TARGET_HERO)
      yawEncoder(),
      yaw(drivers, YAW_L, YAW_R),
#else
      yaw(drivers, YAW, YAW_PID_CONFIG),
#endif
      pitch(drivers, PITCH, PITCH_PID_CONFIG)
{
}

bool TurretSubsystem::hardwareOk() { return yaw.isOnline() && pitch.isOnline(); }

void TurretSubsystem::initialize()
{
    yaw.initialize();
    pitch.initialize();
}

void TurretSubsystem::refresh()
{
    yaw.updateMotorAngle();
    pitch.updateMotorAngle();

    drivers->rtt << "pitch: " << pitch.getAngle();
    drivers->rtt.endl();

#if defined(TARGET_STANDARD) || defined(TARGET_HERO)
    yawEncoder.update();

    if (!isCalibrated && yawEncoder.isOnline())
    {
#if defined(TARGET_HERO)
        baseYaw = -yaw.getAngle();
#else
        baseYaw = yawEncoder.getAngle() - YAW_OFFSET - yaw.getAngle();
#endif
        isCalibrated = true;

        setTargetWorldAngles(getCurrentLocalYaw() + getChassisYaw(), getCurrentLocalPitch());
    }
#else
    if (!isCalibrated && !isAmputated())
    {
        baseYaw = -YAW_OFFSET;
        isCalibrated = true;

        setTargetWorldAngles(getCurrentLocalYaw() + getChassisYaw(), getCurrentLocalPitch());
    }
#endif

    if (isCalibrated && !drivers->isKillSwitched())
    {
        yaw.setAngle(-baseYaw + getTargetLocalYaw(), DT);
        // pitch.motor.setDesiredOutput(GM6020.maxOutput - 1);
        pitch.setAngle((PITCH_OFFSET + getTargetLocalPitch()) * PITCH_REDUCTION, DT);
        drivers->rtt << "target pitch angle "
                     << (PITCH_OFFSET + getTargetLocalPitch()) * PITCH_REDUCTION;
        drivers->rtt.endl();
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

float TurretSubsystem::getChassisYaw() { return drivers->bmi088.getYaw(); }

float TurretSubsystem::getTargetLocalYaw() { return targetWorldYaw - getChassisYaw(); }

float TurretSubsystem::getTargetLocalPitch() { return targetWorldPitch; }

float TurretSubsystem::getCurrentLocalYaw()
{
    return !isCalibrated ? 0.0f : yaw.getAngle() + baseYaw;
}

float TurretSubsystem::getCurrentLocalPitch()
{
    return !isCalibrated ? 0.0f : pitch.getAngle() / PITCH_REDUCTION - PITCH_OFFSET;
}

bool TurretSubsystem::getIsCalibrated() { return isCalibrated; }
}  // namespace subsystems::turret