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
          PITCH_PID_CONFIG,
          4.4f
      ),
      turretOffset(0.0f, 0.0f, M_TWOPI)
{
    initialChassisYaw = modm::toRadian(drivers->bmi088.getYaw());
    initialTurretYaw = yawTurret.getAngle();
    previousYawSetpoint = yawTurret.getSetpoint();
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

float TurretSubsystem::getChassisOffset() {
    float currentChassisYaw = modm::toRadian(drivers->bmi088.getYaw());
    float chassisOffset = tap::algorithms::ContiguousFloat(currentChassisYaw - initialChassisYaw, 0.0f, M_TWOPI).getValue();
    return chassisOffset;
}

float TurretSubsystem::getTurretWithOffset() {
    float chassisOffset = getChassisOffset();
    
    turretOffset.setValue(previousYawSetpoint + initialTurretYaw - (chassisOffset * BELT_RATIO));
    return turretOffset.getValue();
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
}


void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems