#pragma once

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/algorithms/smooth_pid.hpp"
#include "tap/motor/motor_interface.hpp"

#include "drivers/encoder.hpp"
#include "utils/motors/motor_constants.hpp"

#include "drivers.hpp"

namespace subsystems::turret
{
using namespace tap::algorithms;
using namespace tap::motor;
using namespace motors;
using driver::Encoder;

class DoubleYawMotor
{
public:
    DoubleYawMotor(
        src::Drivers *drivers,
        MotorConfig motor1,
        MotorConfig motor2,
        Encoder *encoder,
        const SmoothPidConfig &pidConfig);

    void initialize();
    void reset();
    void updateMotorAngle();
    void setAngle(float desiredAngle, float dt);
    float getAngle();
    float getSetpoint();
    bool isOnline();

private:
    src::Drivers *drivers;
    DjiMotor motor1;
    DjiMotor motor2;
    Encoder *encoder;
    SmoothPid pid;
    ContiguousFloat setpoint;
    ContiguousFloat currentAngle;
};
}  // namespace subsystems::turret