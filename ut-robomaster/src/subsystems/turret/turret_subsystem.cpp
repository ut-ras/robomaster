#include "turret_subsystem.hpp"

#include "tap/algorithms/ballistics.hpp"

#include "modm/math.hpp"
#include "robots/robot_constants.hpp"

using namespace tap::algorithms::ballistics;
namespace subsystems
{
namespace turret
{
TurretSubsystem::TurretSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
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
            yaw = desiredYaw;
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

    if (!isCalibrated && pitchMotor.isMotorOnline() &&
        yawMotor.isMotorOnline())  // calibrate on controller connect
    {
        baseYaw = yawTurret.getAngle();
        basePitch = pitchTurret.getAngle();
        isCalibrated = true;
    }

    if (isCalibrated)
    {
        float world_yaw = (yaw - modm::toRadian(drivers->bmi088.getYaw())) * BELT_RATIO;

        uint32_t time = tap::arch::clock::getTimeMilliseconds();
        uint32_t dt = time - lastTime;
        lastTime = time;

        yawTurret.setAngle(baseYaw + world_yaw, dt);
        pitchTurret.setAngle(basePitch + pitch, dt);
    }
}

void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems