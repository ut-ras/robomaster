#include "double_yaw_motor.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/motor/dji_motor.hpp"

#include "robots/robot_constants.hpp"

namespace subsystems::turret
{
DoubleYawMotor::DoubleYawMotor(src::Drivers* drivers, MotorConfig motor1, MotorConfig motor2)
    : drivers(drivers),
      motor1(drivers, motor1.id, motor1.canBus, motor1.inverted, motor1.name),
      motor2(drivers, motor2.id, motor2.canBus, motor2.inverted, motor2.name),
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
    float encoderAngle =
        static_cast<float>(motor1.getInternalEncoder().getEncoder().getUnwrappedValue()) /
        DjiMotorEncoder::ENC_RESOLUTION / M3508.gearRatio / YAW_REDUCTION;

    currentAngle.setWrappedValue(encoderAngle);
}

void DoubleYawMotor::setAngle(float desiredAngle, float dt)
{
    setpoint.setWrappedValue(desiredAngle / M_TWOPI);

    float positionError = WrappedFloat(currentAngle.getWrappedValue(), 0, 1.0f)
                              .minDifference(setpoint.getWrappedValue());

    // account for chassis rotation
    float disturbance = drivers->bmi088.getGz() / 2.0f / PI;  // rev / s
    disturbance *= 2.0f;                                      // seems to help?

    float targetVelocity = positionPid.update(positionError, dt, false) - disturbance;
    setVelocity(targetVelocity, dt);
}

float DoubleYawMotor::getAngle() { return currentAngle.getWrappedValue() * M_TWOPI; }

void DoubleYawMotor::setVelocity(float velocity, float dt)
{
    float output = velocityPid.update(velocity - getCurrentVelocity(), dt, true);
    setOutput(output);
}

float DoubleYawMotor::getCurrentVelocity()
{
    float rpm1 = motor1.getInternalEncoder().getShaftRPM();                  // rev / m
    float rpm2 = motor2.getInternalEncoder().getShaftRPM();                  // rev / m
    float currentVelocity = (rpm1 + rpm2) / 2.0f / 60.0f / M3508.gearRatio;  // rev / s
    return currentVelocity;
}

void DoubleYawMotor::setOutput(float output)
{
    motor1.setDesiredOutput(output * M3508.maxOutput);
    motor2.setDesiredOutput(output * M3508.maxOutput);
}

float DoubleYawMotor::getOutput() { return motor1.getOutputDesired() / M3508.maxOutput; }

float DoubleYawMotor::getSetpoint() { return setpoint.getWrappedValue(); }
bool DoubleYawMotor::isOnline() { return motor1.isMotorOnline() && motor2.isMotorOnline(); }
}  // namespace subsystems::turret