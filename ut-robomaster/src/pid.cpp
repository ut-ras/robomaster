#include "pid.hpp"

namespace pid_motor_controller
{
Pid::Pid(const float& kp, const float& ki, const float& kd) : kp(kp), ki(ki), kd(kd) {}

float Pid::update(float target, float measured, float dt)
{
    error[2] = error[1];
    error[1] = error[0];
    error[0] = target - measured;

    output += (kp + ki * dt + kd / dt) * error[0];
    output -= (kp + 2.0f * kd / dt) * error[1];
    output += (kd / dt) * error[2];

    if (output > 1.0f)
        output = 1.0f;
    else if (output < -1.0f)
        output = -1.0f;

    return output;
}
}  // namespace pid_motor_controller