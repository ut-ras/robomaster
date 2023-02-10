#include "turret_subsystem.hpp"

#include "robots/robot_constants.hpp"

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

void TurretSubsystem::setDesiredRpm(float yaw, float pitch)
{
    desiredRpmYaw = yaw;
    desiredRpmPitch = pitch;
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
    yawMotor.update(desiredRpmYaw / 60.0f);
    pitchMotor.update(desiredRpmPitch / 60.0f);
}

void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems