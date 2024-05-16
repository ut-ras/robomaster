#include "double_yaw_motor.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/motor/dji_motor.hpp"

namespace subsystems::turret
{

DoubleYawMotor::DoubleYawMotor(
    src::Drivers *drivers,
    MotorInterface *motor1,
    MotorInterface *motor2,
    Encoder *encoder,
    const SmoothPidConfig &pidConfig)
    : drivers(drivers),
      motor1(motor1),
      motor2(motor2),
      encoder(encoder),
      pid(pidConfig),
      setpoint(0.0f, 0.0f, M_TWOPI),
      currentAngle(0.0f, 0.0f, M_TWOPI)
{
}

void DoubleYawMotor::initialize()
{
    motor1->initialize();
    motor2->initialize();
}

void DoubleYawMotor::reset()
{
    motor1->setDesiredOutput(0);
    motor2->setDesiredOutput(0);
    pid.reset();
}

void DoubleYawMotor::updateMotorAngle()
{
    float encoderAngle = encoder->getAngle() * M_TWOPI;
    currentAngle.setValue(encoderAngle);
}

void DoubleYawMotor::setAngle(float desiredAngle, float dt)
{
    setpoint.setValue(desiredAngle);

    float positionControllerError =
        ContiguousFloat(currentAngle.getValue(), 0, M_TWOPI).difference(setpoint.getValue());
    float rpm1 = motor1->getShaftRPM();                       // rev / m
    float rpm2 = motor2->getShaftRPM();                       // rev / m
    float avgRps = (rpm1 + rpm2) / 2.0f * (M_TWOPI / 60.0f);  // rad / s
    float output = pid.runController(positionControllerError, avgRps, dt);

    output = limitVal(output, -GM6020_MAX_OUTPUT, GM6020_MAX_OUTPUT);
    motor1->setDesiredOutput(output);
    motor2->setDesiredOutput(output);
}

float DoubleYawMotor::getAngle() { return currentAngle.getValue(); }

float DoubleYawMotor::getSetpoint() { return setpoint.getValue(); }
bool DoubleYawMotor::isOnline() { return motor1->isMotorOnline() && motor2->isMotorOnline(); }
}  // namespace subsystems::turret