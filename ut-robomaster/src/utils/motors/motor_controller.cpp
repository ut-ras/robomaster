#include "motor_controller.hpp"

#include "tap/algorithms/math_user_utils.hpp"

#include "robots/robot_constants.hpp"

using tap::algorithms::limitVal;

namespace motors
{
void MotorController::initialize() { motor.initialize(); }

void MotorController::setActive(bool active)
{
    isActive = active;
    if (!active)
    {
        positionPid.reset();
        velocityPid.reset();
        motor.setDesiredOutput(0);
    }
}

void MotorController::updatePosition(float target)
{
    if (!isActive) return;

    float diff = target - measurePosition();
    float smallest_diff = diff - roundf(diff);
    float velocity = positionPid.update(smallest_diff, DT, false);
    updateVelocity(velocity);
}

void MotorController::updateVelocity(float target)
{
    if (!isActive) return;

    float diff = target - measureVelocity();
    float output = velocityPid.update(diff, DT, true);
    setOutput(output);
}

void MotorController::setOutput(float output)
{
    float clampedOutput = limitVal(output, -1.0f, 1.0f);
    motor.setDesiredOutput(clampedOutput * constants.maxOutput);
}

bool MotorController::isOnline() { return motor.isMotorOnline(); }

float MotorController::measurePosition()
{
    int64_t encoderVal = motor.getEncoderUnwrapped();
    float units = static_cast<float>(encoderVal) / DjiMotor::ENC_RESOLUTION;
    float turns = units / constants.gearRatio;  // revs
    return turns;
}

float MotorController::measureVelocity()
{
    int16_t rpm = motor.getShaftRPM() / constants.gearRatio;
    float rps = rpm / 60.0f;  // revs / sec
    return rps;
}

void MotorController::applyPowerScalar(float powerLimitScalar)
{
    if (!isActive) return;
    float invertFactor = motor.isMotorInverted() ? -1.0f : 1.0f;
    motor.setDesiredOutput(motor.getOutputDesired() * invertFactor * powerLimitScalar);
}
}  // namespace motors