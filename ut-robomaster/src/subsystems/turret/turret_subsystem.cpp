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
      yawMotor(drivers, GM6020, ID_YAW, CAN_TURRET, false, "yaw", PID_KP, PID_KI, PID_KD),
      pitchMotor(drivers, M3508, ID_PITCH, CAN_TURRET, false, "pitch", PID_KP, PID_KI, PID_KD)
{
}

void TurretSubsystem::initialize()
{
    yawMotor.initialize();
    pitchMotor.initialize();
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

    yawMotor.update(yaw);
    pitchMotor.update(pitch);
}

void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems