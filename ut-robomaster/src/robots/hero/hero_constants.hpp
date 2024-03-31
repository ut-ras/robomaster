#pragma once

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/container/pair.hpp"
#include "subsystems/sound/sounds/sound_smb_powerup.hpp"
#include "utils/motor_controller/pid.hpp"

using motor_controller::PidConstants;
using tap::can::CanBus;
using namespace tap::motor;

constexpr CanBus CAN_WHEELS = CanBus::CAN_BUS1;
constexpr CanBus CAN_TURRET = CanBus::CAN_BUS1;
constexpr CanBus CAN_SHOOTER = CanBus::CAN_BUS2;

constexpr MotorId ID_WHEEL_LF = MOTOR2;
constexpr MotorId ID_WHEEL_RF = MOTOR1;
constexpr MotorId ID_WHEEL_LB = MOTOR3;
constexpr MotorId ID_WHEEL_RB = MOTOR4;

static constexpr int WHEELS = 4;
static constexpr float WHEEL_DISTANCE_X = 0.525f;  // meters
static constexpr float WHEEL_DISTANCE_Y = 0.400f;  // meters
static constexpr float WHEEL_RADIUS = 0.1524f;     // meters
static constexpr float WHEEL_LXY = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;
static constexpr float WHEEL_MAX_VEL = 120.0f;                                      // rad/s
static constexpr float MAX_LINEAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS;               // m/s
static constexpr float MAX_ANGULAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS / WHEEL_LXY;  // rad/s

constexpr PidConstants PID_VELOCITY_DEFAULT = {0.75f, 12.0f, 0.0f};
constexpr PidConstants PID_WHEELS = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_VELOCITY = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_POSITION = {0.1f, 0.1f, 0.0f};

constexpr MotorId ID_FLYWHEEL_L = MOTOR3;
constexpr MotorId ID_FLYWHEEL_R = MOTOR4;
constexpr MotorId ID_AGITATOR = MOTOR1;
constexpr MotorId ID_FEEDER = MOTOR2;

constexpr MotorId ID_YAW = MOTOR5;
constexpr MotorId ID_PITCH = MOTOR6;

constexpr PidConstants PID_FLYWHEEL = {0.1f, 0.1f, 0.0f};
constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_FEEDER = PID_VELOCITY_DEFAULT;

constexpr bool USE_BALLISTICS = false;
constexpr int BALLISTIC_ITERATIONS = 2;

constexpr float PITCH_MIN = -0.3349f;  // rad
constexpr float PITCH_MAX = 0.3534f;   // rad

// NEED TO MEASURE (MAYBE ADD NEW CONSTANTS)
constexpr float CAMERA_TO_PITCH = 0.0f;    // distance from main camera lens to pitch axis (m)
constexpr float NOZZLE_TO_PITCH = 0.0f;    // distance from barrel nozzle to pitch axis (m)
constexpr float CAMERA_TO_BARRELS = 0.0f;  // vertical ctc offset from camera lens to barrel (m)
constexpr float CAMERA_X_OFFSET = 0.0f;    // horizontal offset of main camera lens (m)

static constexpr modm::Pair<uint16_t, float> FLYWHEEL_RPS_MAPPING[] = {{10, 105.0f}, {16, 190.0f}};

const float BALLS_PER_SEC = 4.0f;
const float BALLS_PER_REV = 6.0f;
const float FEEDER_RATIO = 1.0f;  // feeder speed / agitator speed

const uint16_t BARREL_HEAT_BUFFER = 100.0f;

const float JAM_TRIGGER_RATIO = 0.5;      // measured speed to driven speed ratio
const float JAM_TRIGGER_DURATION = 0.1f;  // s
const float UNJAM_DURATION = 0.1f;        // s
const float UNJAM_SPEED = 12.0f;          // rev/s

static constexpr int FLYWHEELS = 2;
static constexpr float DEFAULT_SPEED = 60.0f;

static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
    .kp = 100'183.1f,
    .ki = 0.0f,
    .kd = 2'500.0f,
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
    .maxOutput = 32'000.0f,
    .tQDerivativeKalman = 1.0f,
    .tRDerivativeKalman = 30.0f,
    .tQProportionalKalman = 1.0f,
    .tRProportionalKalman = 0.0f,
    .errDeadzone = 0.0f,
    .errorDerivativeFloor = 0.0f,
};

static constexpr float YAW_REDUCTION = 2.0f;
static constexpr float PITCH_REDUCTION = 1.0f;

static constexpr float YAW_INPUT_SCALE = 4.0f;
static constexpr float PITCH_INPUT_SCALE = 5.0f;

static constexpr float MOUSE_SENS_YAW = 0.0045f;
static constexpr float MOUSE_SENS_PITCH = 0.002f;

const Sound SOUND_STARTUP = SOUND_SMB_POWERUP;