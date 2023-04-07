#pragma once

#include "tap/motor/motor_interface.hpp"
#include "tap/algorithms/smooth_pid.hpp"
#include "tap/algorithms/contiguous_float.hpp"

namespace src {
    class Drivers;
}

namespace subsystems::turret{

class TurretMotor{

public:
    TurretMotor(src::Drivers *drivers, tap::motor::MotorInterface *motor, const tap::algorithms::SmoothPidConfig &pidConfig);

    TurretMotor(src::Drivers *drivers, tap::motor::MotorInterface *motor, const tap::algorithms::SmoothPidConfig &pidConfig, float startAngle);
    void initialize();
    void updateMotorAngle();
    void setAngle(float desiredAngle, uint32_t dt);
    float getAngle();
    float getSetpoint();

private:
    src::Drivers *drivers;
    tap::motor::MotorInterface *motor;
    tap::algorithms::SmoothPid pid;
    tap::algorithms::ContiguousFloat setpoint;
    float unwrappedAngle = 0;
    tap::algorithms::ContiguousFloat currentAngle;
    float lastUpdatedEncoderValue = 0;

    static constexpr float GM6020_MAX_OUTPUT = 30000.0f;
};
}