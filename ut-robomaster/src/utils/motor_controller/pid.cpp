#include "pid.hpp"

namespace motor_controller
{
Pid::Pid(const PidConstants& constants) : k(constants) {}

float Pid::update(float err, float dt)
{
    error[2] = error[1];
    error[1] = error[0];
    error[0] = err;

    output += (k.p + k.i * dt + k.d / dt) * error[0];
    output -= (k.p + 2.0f * k.d / dt) * error[1];
    output += (k.d / dt) * error[2];

    if (output > 1.0f)
        output = 1.0f;
    else if (output < -1.0f)
        output = -1.0f;

    lastError = output;
    return output;
}

void Pid::reset()
{
    error[0] = 0;
    error[1] = 0;
    error[2] = 0;
    output = 0.0f;
}

}  // namespace motor_controller