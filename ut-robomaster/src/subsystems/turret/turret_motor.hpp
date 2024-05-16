#pragma once

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/algorithms/smooth_pid.hpp"
#include "tap/motor/motor_interface.hpp"

#include "utils/motors/motor_constants.hpp"

#include "drivers.hpp"

namespace subsystems::turret
{
using namespace tap::algorithms;
using namespace tap::motor;
using motors::MotorConfig;

class TurretMotor
{
public:
    TurretMotor(src::Drivers *drivers, MotorConfig motor, const SmoothPidConfig &pidConfig);

    void initialize();
    void reset();
    void updateMotorAngle();
    void setAngle(float desiredAngle, float dt);
    float getAngle();
    float getSetpoint();
    bool isOnline();

private:
    src::Drivers *drivers;
    DjiMotor motor;
    SmoothPid pid;
    ContiguousFloat setpoint;
    float unwrappedAngle = 0;
    ContiguousFloat currentAngle;
    float lastUpdatedEncoderValue = 0;

    static constexpr float GM6020_MAX_OUTPUT = 30000.0f;
};
}  // namespace subsystems::turret