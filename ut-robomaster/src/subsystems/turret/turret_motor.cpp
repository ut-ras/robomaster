#include "turret_motor.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/motor/dji_motor.hpp"

using namespace tap::motor;
using namespace tap::algorithms;

namespace subsystems::turret
{

TurretMotor::TurretMotor(
    src::Drivers *drivers,
    tap::motor::MotorInterface *motor,
    const tap::algorithms::SmoothPidConfig &pidConfig)
    : drivers(drivers),
      motor(motor),
      pid(pidConfig),
      setpoint(0.0f, 0.0f, M_TWOPI),
      currentAngle(0.0f, 0.0f, M_TWOPI)
{
}

TurretMotor::TurretMotor(
    src::Drivers *drivers,
    tap::motor::MotorInterface *motor,
    const tap::algorithms::SmoothPidConfig &pidConfig,
    float startAngle,
    float lowerRange,
    float upperRange)
    : drivers(drivers),
      motor(motor),
      pid(pidConfig),
      setpoint(startAngle, 0.0f, M_TWOPI),
      currentAngle(startAngle, 0.0f, M_TWOPI),
      lowerRange(lowerRange),
      upperRange(upperRange)
{
}

void TurretMotor::initialize() { motor->initialize(); }

void TurretMotor::updateMotorAngle()
{
    uint16_t encoderValue = motor->getEncoderWrapped();
    if (lastUpdatedEncoderValue != encoderValue)
    {
        lastUpdatedEncoderValue = encoderValue;

        unwrappedAngle = static_cast<float>(encoderValue) * M_TWOPI /
                         static_cast<float>(DjiMotor::ENC_RESOLUTION);
        currentAngle.setValue(unwrappedAngle);
    }
}

void TurretMotor::setAngle(float desiredAngle, uint32_t dt)
{
    if (lowerRange != -1.0f && upperRange != -1.0f)
    {
        if (desiredAngle < lowerRange || desiredAngle > upperRange)
        {
            return;
        }
    }

    setpoint.setValue(desiredAngle);

    float positionControllerError =
        ContiguousFloat(currentAngle.getValue(), 0, M_TWOPI).difference(setpoint.getValue());
    // drivers->terminal << currentAngle.getValue() << "\n";
    // drivers->terminal << desiredAngle << "\n";

    float output =
        pid.runController(positionControllerError, (M_TWOPI / 60.0f) * motor->getShaftRPM(), dt);

    output = limitVal(output, -GM6020_MAX_OUTPUT, GM6020_MAX_OUTPUT);
    motor->setDesiredOutput(output);
}

float TurretMotor::getAngle() { return currentAngle.getValue(); }

float TurretMotor::getSetpoint() { return setpoint.getValue(); }
}  // namespace subsystems::turret