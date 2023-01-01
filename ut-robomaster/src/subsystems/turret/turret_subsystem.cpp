#include "turret_subsystem.hpp"

namespace subsystems
{
namespace turret
{
TurretSubsystem::TurretSubsystem(tap::Drivers* drivers, MotorId motorIdYaw, MotorId motorIdPitch)
    : tap::control::Subsystem(drivers),
      velocityPidYawMotor(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
      velocityPidPitchMotor(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
      yawMotor(drivers, motorIdYaw, CAN_BUS_MOTORS, false, "yaw motor"),
      pitchMotor(drivers, motorIdPitch, CAN_BUS_MOTORS, false, "pitch motor"),
      agitator(
          DjiMotor(drivers, MOTOR5, CAN_BUS_MOTORS, false, "agitator motor"),
          30000,
          1.0f,
          0.0f,
          0.0f)
{
}

void TurretSubsystem::initialize()
{
    yawMotor.initialize();
    pitchMotor.initialize();
    agitator.init();
}

void TurretSubsystem::setDesiredRpm(float yaw, float pitch)
{
    desiredRpmYaw = yaw;
    desiredRpmPitch = pitch;
}

void TurretSubsystem::refresh()
{
    agitator.update(desiredRpmYaw);

    // updateMotorRpmPid(&velocityPidYawMotor, &yawMotor, desiredRpmYaw);
    // updateMotorRpmPid(&velocityPidPitchMotor, &pitchMotor, desiredRpmPitch);
}

void TurretSubsystem::updateMotorRpmPid(modm::Pid<float>* pid, DjiMotor* motor, float desiredRpm)
{
    pid->update(desiredRpm - motor->getShaftRPM());
    motor->setDesiredOutput(static_cast<int32_t>(pid->getValue()));
}

void TurretSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace turret
}  // namespace subsystems