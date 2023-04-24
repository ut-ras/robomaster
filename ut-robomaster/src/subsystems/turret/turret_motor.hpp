#pragma once

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/algorithms/smooth_pid.hpp"
#include "tap/motor/motor_interface.hpp"

namespace src
{
class Drivers;
}

namespace subsystems::turret
{
using namespace tap::algorithms;
using namespace tap::motor;

class TurretMotor
{
public:
    TurretMotor(src::Drivers *drivers, MotorInterface *motor, const SmoothPidConfig &pidConfig);

    void initialize();
    void reset();
    void updateMotorAngle();
    void setAngle(float desiredAngle, uint32_t dt);
    float getAngle();
    float getSetpoint();

private:
    src::Drivers *drivers;
    MotorInterface *motor;
    SmoothPid pid;
    ContiguousFloat setpoint;
    float unwrappedAngle = 0;
    ContiguousFloat currentAngle;
    float lastUpdatedEncoderValue = 0;

    static constexpr float GM6020_MAX_OUTPUT = 30000.0f;
};
}  // namespace subsystems::turret