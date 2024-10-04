#include "double_yaw_motor.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/motor/dji_motor.hpp"

namespace subsystems::turret
{
DoubleYawMotor::DoubleYawMotor(
    src::Drivers *drivers,
    MotorConfig motor1,
    MotorConfig motor2,
    Encoder *encoder)
    : drivers(drivers),
      motor1(drivers, motor1.id, motor1.canBus, motor1.inverted, motor1.name),
      motor2(drivers, motor2.id, motor2.canBus, motor2.inverted, motor2.name),
      encoder(encoder),
      velocityPid(motor1.velocityPidConstants),
      positionPid(motor1.positionPidConstants),
      setpoint(0.0f, 0.0f, 1.0f),
      currentAngle(0.0f, 0.0f, 1.0f)
{
}

void DoubleYawMotor::initialize()
{
    motor1.initialize();
    motor2.initialize();
    isCalibrated = false;
}

void DoubleYawMotor::reset()
{
    motor2.setDesiredOutput(0);
    motor1.setDesiredOutput(0);
    velocityPid.reset();
    positionPid.reset();
}

void DoubleYawMotor::updateMotorAngle()
{
    if (!isCalibrated && encoder->isOnline())
    {
        initialAngle = encoder->getAngle();
        isCalibrated = true;
    }
    float encoderAngle = static_cast<float>(motor1.getEncoderUnwrapped()) /
                         DjiMotor::ENC_RESOLUTION / M3508.gearRatio / 2.0f;
    currentAngle.setValue(encoderAngle);
}

void DoubleYawMotor::setAngle(float desiredAngle, float dt)
{
    setpoint.setValue(desiredAngle / M_TWOPI);

    float positionError =
        ContiguousFloat(currentAngle.getValue(), 0, 1.0f).difference(setpoint.getValue());

    // account for chassis rotation
    float disturbance = drivers->bmi088.getGz() / 360.0f;  // rev / s
    disturbance *= 2.0f;                                   // seems to help?

    float targetVelocity = positionPid.update(positionError, dt, false) - disturbance;
    setVelocity(targetVelocity, dt);
}

float DoubleYawMotor::getAngle() { return currentAngle.getValue() * M_TWOPI; }

void DoubleYawMotor::setVelocity(float velocity, float dt)
{
    float output = velocityPid.update(velocity - getCurrentVelocity(), dt, true);
    setOutput(output);
}

float DoubleYawMotor::getCurrentVelocity()
{
    float rpm1 = motor1.getShaftRPM();                                       // rev / m
    float rpm2 = motor2.getShaftRPM();                                       // rev / m
    float currentVelocity = (rpm1 + rpm2) / 2.0f / 60.0f / M3508.gearRatio;  // rev / s
    return currentVelocity;
}

void DoubleYawMotor::setOutput(float output)
{
    motor1.setDesiredOutput(output * M3508.maxOutput);
    motor2.setDesiredOutput(output * M3508.maxOutput);
}

float DoubleYawMotor::getOutput() { return motor1.getOutputDesired() / M3508.maxOutput; }

float DoubleYawMotor::getSetpoint() { return setpoint.getValue(); }
bool DoubleYawMotor::isOnline() { return motor1.isMotorOnline() && motor2.isMotorOnline(); }
}  // namespace subsystems::turret