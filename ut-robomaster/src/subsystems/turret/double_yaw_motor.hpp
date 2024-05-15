#pragma once

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/algorithms/smooth_pid.hpp"
#include "tap/motor/motor_interface.hpp"

#include "drivers/encoder.hpp"

#include "drivers.hpp"

namespace subsystems::turret
{
using namespace tap::algorithms;
using namespace tap::motor;
using driver::Encoder;

class DoubleYawMotor
{
public:
    DoubleYawMotor(
        src::Drivers *drivers,
        MotorInterface *motor1,
        MotorInterface *motor2,
        Encoder *encoder,
        const SmoothPidConfig &pidConfig);

    void initialize();
    void reset();
    void updateMotorAngle();
    void setAngle(float desiredAngle, float dt);
    float getAngle();
    float getSetpoint();

private:
    src::Drivers *drivers;
    MotorInterface *motor1;
    MotorInterface *motor2;
    Encoder *encoder;
    SmoothPid pid;
    ContiguousFloat setpoint;
    ContiguousFloat currentAngle;

    static constexpr float GM6020_MAX_OUTPUT = 30000.0f;
};
}  // namespace subsystems::turret