namespace pid_motor_controller
{
struct MotorConstants
{
    uint16_t maxOutput;
    float gearRatio;
};

constexpr MotorConstants M3508{16384, 3591.0f / 187.0f};

}  // namespace pid_motor_controller