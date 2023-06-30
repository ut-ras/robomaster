#pragma once

#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/algorithms/smooth_pid.hpp"

#include "modm/container/pair.hpp"

#include "utils/motor_controller/pid.hpp"


using motor_controller::PidConstants;
using tap::can::CanBus;
using namespace tap::motor;

// General constants ------------------------------------------------
constexpr CanBus CAN_WHEELS = CanBus::CAN_BUS1;
constexpr CanBus CAN_TURRET = CanBus::CAN_BUS1;
constexpr CanBus CAN_SHOOTER = CanBus::CAN_BUS2;

constexpr PidConstants PID_VELOCITY_DEFAULT = {0.75f, 12.0f, 0.0f};

// Chassis constants ------------------------------------------------
constexpr MotorId ID_WHEEL_LF = MOTOR2;
constexpr MotorId ID_WHEEL_RF = MOTOR1;
constexpr MotorId ID_WHEEL_LB = MOTOR3;
constexpr MotorId ID_WHEEL_RB = MOTOR4;

static constexpr int WHEELS = 4;
static constexpr float WHEEL_DISTANCE_X = 0.391f;  // meters
static constexpr float WHEEL_DISTANCE_Y = 0.315f;  // meters
static constexpr float WHEEL_RADIUS = 0.1524f;  // meters
static constexpr float WHEEL_LXY = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;
static constexpr float WHEEL_MAX_VEL = 10.0f;                                       // rad/s
static constexpr float MAX_LINEAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS;               // m/s
static constexpr float MAX_ANGULAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS / WHEEL_LXY;  // rad/s

constexpr PidConstants PID_WHEELS = PID_VELOCITY_DEFAULT;

// Flywheel constants ------------------------------------------------
constexpr MotorId ID_FLYWHEEL_TL = MOTOR3;
constexpr MotorId ID_FLYWHEEL_TR = MOTOR4;
constexpr MotorId ID_FLYWHEEL_BL = MOTOR5;
constexpr MotorId ID_FLYWHEEL_BR = MOTOR6;

constexpr PidConstants PID_FLYWHEEL = {0.1f, 0.1f, 0.0f};

static constexpr modm::Pair<uint16_t, float> FLYWHEEL_RPS_MAPPING[] = {
    {15, 70.0f},
    {18, 78.57f},
    {30, 125.0f}};

static constexpr int FLYWHEELS = 4;
static constexpr float DEFAULT_SPEED = 70.0f;

// Agitator constants ------------------------------------------------
constexpr MotorId ID_AGITATOR_L = MOTOR1;
constexpr MotorId ID_AGITATOR_R = MOTOR2;

constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;

const float BALLS_PER_SEC = 6.0f;
const float BALLS_PER_REV = 8.0f;

const uint16_t BARREL_HEAT_BUFFER = 20.0f;

const float UNJAM_SPEED = 0.4f;  // rev/s

// Turret constants ------------------------------------------------
constexpr MotorId ID_YAW = MOTOR6;
constexpr MotorId ID_PITCH = MOTOR7;

constexpr float PITCH_MIN = -0.2185f;
constexpr float PITCH_MAX = 0.2299f;

constexpr PidConstants PID_TURRET_VELOCITY = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_POSITION = {0.1f, 0.1f, 0.0f};

static constexpr float BELT_RATIO = 2.0f;

static constexpr float YAW_INPUT_SCALE = 0.02f;
static constexpr float PITCH_INPUT_SCALE = 0.01f;

static constexpr float MOUSE_SENS_YAW = 0.0002f;
static constexpr float MOUSE_SENS_PITCH = 0.0002f;

static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
    .kp = 65'000.0f,
    .ki = 0.0f,
    .kd = 3'000.0f,
    .maxICumulative = 0.0f,
    .maxOutput = 32'000.0f,
    .tQDerivativeKalman = 1.0f,
    .tRDerivativeKalman = 30.0f,
    .tQProportionalKalman = 1.0f,
    .tRProportionalKalman = 0.0f,
    .errDeadzone = 0.0f,
    .errorDerivativeFloor = 10.0f,
};

static constexpr tap::algorithms::SmoothPidConfig PITCH_PID_CONFIG = {
    .kp = 100'183.1f,
    .ki = 0.0f,
    .kd = 1'000.0f,
    .maxICumulative = 0.0f,
    .maxOutput = 32000.0f,
    .tQDerivativeKalman = 1.0f,
    .tRDerivativeKalman = 30.0f,
    .tQProportionalKalman = 1.0f,
    .tRProportionalKalman = 0.0f,
    .errDeadzone = 0.0f,
    .errorDerivativeFloor = 0.0f,
};