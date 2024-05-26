#pragma once

#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"

#include "pid.hpp"

namespace motors
{
using motor_controller::PidConstants;

struct MotorSpecs
{
    uint16_t maxOutput;
    float gearRatio;
};

struct MotorConfig
{
    MotorSpecs constants;
    tap::motor::MotorId id;
    tap::can::CanBus canBus;
    bool inverted;
    const char* const name;
    PidConstants pidConstants;
};

// C620 controller (16384 = 20A)
// Max rated motor current is 10A
constexpr MotorSpecs M3508{16384 / 2, 3591.0f / 187.0f};
constexpr MotorSpecs M3508_NOGEARBOX{M3508.maxOutput, 1.0f};

// C610 controller (10000 = 10A)
// Max continuous current is 10A
constexpr MotorSpecs M2006{10000, 36.0f / 1.0f};
constexpr MotorSpecs M2006_NOGEARBOX{M2006.maxOutput, 1.0f};

// No external controller
// Motor is voltage-controlled (max 24V)
constexpr MotorSpecs GM6020{30000, 1.0f};

}  // namespace motors