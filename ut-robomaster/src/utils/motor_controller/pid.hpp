#ifndef UTILS_MOTOR_CONTROLLER_PID_HPP_
#define UTILS_MOTOR_CONTROLLER_PID_HPP_

namespace motor_controller
{
struct PidConstants
{
    float p;
    float i;
    float d;
};

class Pid
{
public:
    Pid(const PidConstants& constants);
    float update(float err, float dt);
    void reset();
    float getLastError() { return lastError; }

    const PidConstants k;

    // private:
    float error[3] = {0.0f, 0.0f, 0.0f};
    float output = 0.0f;
private:
    float lastError = 0.0f;
};
}  // namespace motor_controller

#endif