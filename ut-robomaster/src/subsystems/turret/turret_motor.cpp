#include "turret_motor.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/motor/dji_motor.hpp"

namespace subsystems::turret
{
TurretMotor::TurretMotor(
    src::Drivers *drivers,
    MotorConfig motorConfig,
    const SmoothPidConfig &pidConfig)
    : motor(drivers, motorConfig.id, motorConfig.canBus, motorConfig.inverted, motorConfig.name),
      drivers(drivers),
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
        currentAngle.setWrappedValue(unwrappedAngle);
    }
}

void TurretMotor::setAngle(float desiredAngle, float dt)
{
    setpoint.setWrappedValue(desiredAngle);

    float positionControllerError = WrappedFloat(currentAngle.getWrappedValue(), 0, M_TWOPI)
                                        .minDifference(setpoint.getWrappedValue());
    float output =
        pid.runController(positionControllerError, (M_TWOPI / 60.0f) * motor.getShaftRPM(), dt);

    motor.setDesiredOutput(output);
}

float TurretMotor::getAngle() { return currentAngle.getWrappedValue(); }

float TurretMotor::getSetpoint() { return setpoint.getWrappedValue(); }

bool TurretMotor::isOnline() { return motor.isMotorOnline(); }
}  // namespace subsystems::turret