namespace motor_controller
{
struct MotorConstants
{
    uint16_t maxOutput;
    float gearRatio;
};

// C620 controller (16384 = 20A)
// Max rated motor current is 10A
constexpr MotorConstants M3508{16384 / 2, 3591.0f / 187.0f};
constexpr MotorConstants M3508_NOGEARBOX{M3508.maxOutput, 1.0f};

// C610 controller (10000 = 10A)
// Max continuous current is 10A
constexpr MotorConstants M2006{10000, 36.0f / 1.0f};
constexpr MotorConstants M2006_NOGEARBOX{M2006.maxOutput, 1.0f};

// No external controller
// Motor is voltage-controlled (max 24V)
constexpr MotorConstants GM6020{30000, 1.0f};

}  // namespace motor_controller