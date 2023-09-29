#pragma once

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/container/pair.hpp"
#include "utils/motor_controller/pid.hpp"

using motor_controller::PidConstants;
using tap::can::CanBus;
using namespace tap::motor;

// General constants ------------------------------------------------
constexpr CanBus CAN_AGITATOR = CanBus::CAN_BUS1;

constexpr MotorId ID_AGITATOR = MOTOR1;
constexpr PidConstants PID_AGITATOR = {0.75f, 12.0f, 0.0f};

const float MAX_SPEED = 140.0f / 60.0f;