#include "turret_motor.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/motor/dji_motor.hpp"

namespace subsystems::turret
{
TurretMotor::TurretMotor(
    src::Drivers *drivers,
    MotorConfig motorConfig,
    const SmoothPidConfig &pidConfig)
    : drivers(drivers),
      motor(drivers, motorConfig.id, motorConfig.canBus, motorConfig.inverted, motorConfig.name),
      pid(pidConfig),
      setpoint(0.0f, 0.0f, M_TWOPI),
      currentAngle(0.0f, 0.0f, M_TWOPI)
{
}

void TurretMotor::initialize() { motor.initialize(); }

void TurretMotor::reset()
{
    pid.reset();
    motor.setDesiredOutput(0);
}

void TurretMotor::updateMotorAngle()
{
    uint16_t encoderValue = motor.getEncoderWrapped();
    if (lastUpdatedEncoderValue != encoderValue)
    {
        lastUpdatedEncoderValue = encoderValue;

        unwrappedAngle = static_cast<float>(encoderValue) * M_TWOPI /
                         static_cast<float>(DjiMotor::ENC_RESOLUTION);
        currentAngle.setValue(unwrappedAngle);
    }
}

void TurretMotor::setAngle(float desiredAngle, float dt)
{
    setpoint.setValue(desiredAngle);

    float positionControllerError =
        ContiguousFloat(currentAngle.getValue(), 0, M_TWOPI).difference(setpoint.getValue());
    float output =
        pid.runController(positionControllerError, (M_TWOPI / 60.0f) * motor.getShaftRPM(), dt);

    output = limitVal(output, -GM6020_MAX_OUTPUT, GM6020_MAX_OUTPUT);
    motor.setDesiredOutput(output);
}

float TurretMotor::getAngle() { return currentAngle.getValue(); }

float TurretMotor::getSetpoint() { return setpoint.getValue(); }

bool TurretMotor::isOnline() { return motor.isMotorOnline(); }
}  // namespace subsystems::turret