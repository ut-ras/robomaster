namespace pid_motor_controller
{

class Pid
{
public:
    Pid(const float& kp = 1.0f, const float& ki = 0.0f, const float& kd = 0.0f);
    float update(float target, float measured, float dt);

    // private:
    const float kp;
    const float ki;
    const float kd;
    float error[3] = {0.0f, 0.0f, 0.0f};
    float output = 0.0f;
};
}  // namespace pid_motor_controller