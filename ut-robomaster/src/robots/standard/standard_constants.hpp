#pragma once

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/container/pair.hpp"
#include "subsystems/sound/sounds/sound_smb_powerup.hpp"
#include "utils/motors/motor_constants.hpp"
#include "utils/motors/pid.hpp"

using motor_controller::PidConstants;
using tap::can::CanBus;
using namespace tap::motor;
using namespace motors;

// General constants ------------------------------------------------

constexpr bool USE_BALLISTICS = false;
constexpr int BALLISTIC_ITERATIONS = 2;

const Sound SOUND_STARTUP = SOUND_SMB_POWERUP;

// Kinematic constants -----------------------------------------------

// chassis ------------
static constexpr int WHEELS = 4;
static constexpr float WHEEL_DISTANCE_X = 0.391f;  // meters
static constexpr float WHEEL_DISTANCE_Y = 0.315f;  // meters
static constexpr float WHEEL_RADIUS = 0.1524f;     // meters
static constexpr float WHEEL_LXY = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;

// flywheels -----------
static constexpr int FLYWHEELS = 2;

// turret ------------
constexpr float PITCH_MIN = 0.10f;            // rad
constexpr float PITCH_MAX = 0.79f;            // rad
constexpr float CAMERA_TO_PITCH = 0.13555f;   // distance from main camera lens to pitch axis (m)
constexpr float NOZZLE_TO_PITCH = 0.18151f;   // distance from barrel nozzle to pitch axis (m)
constexpr float CAMERA_TO_BARRELS = 0.0427f;  // vertical ctc offset from camera lens to barrel (m)
constexpr float CAMERA_X_OFFSET = -0.0335f;   // horizontal offset of main camera lens (m)

static constexpr float YAW_REDUCTION = 1.0f;
static constexpr float PITCH_REDUCTION = 1.0f;

// Tuning constants -----------------------------------------------

constexpr PidConstants PID_VELOCITY_DEFAULT = {0.75f, 12.0f, 0.0f};
constexpr PidConstants PID_POSITION_DEFAULT = {10.0f, 0.0f, 0.0f};
constexpr PidConstants PID_WHEELS = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_FLYWHEEL = {0.065f, 0.325f, 0.0f};
constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_VELOCITY = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_POSITION = {0.1f, 0.1f, 0.0f};

static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
    .kp = 100'000.0f,
    .ki = 0.0f,
    .kd = 150.0f,
    .maxICumulative = 0.0f,
    .maxOutput = M3508.maxOutput,
    .tQDerivativeKalman = 1.0f,
    .tRDerivativeKalman = 0.0f,
    .tQProportionalKalman = 1.0f,
    .tRProportionalKalman = 0.0f,
    .errDeadzone = 0.0f,
    .errorDerivativeFloor = 0.0f,
};

static constexpr tap::algorithms::SmoothPidConfig PITCH_PID_CONFIG = {
    .kp = 100'000.0f,
    .ki = 100'000.0f,
    .kd = 3'500.0f,
    .maxICumulative = 0.0f,
    .maxOutput = GM6020.maxOutput,
    .tQDerivativeKalman = 1.0f,
    .tRDerivativeKalman = 30.0f,
    .tQProportionalKalman = 1.0f,
    .tRProportionalKalman = 0.0f,
    .errDeadzone = 0.0f,
    .errorDerivativeFloor = 0.0f,
};

// Input constants ----------------------------

#ifdef DEMO_MODE
static constexpr float YAW_INPUT_SCALE = 4.0f;
static constexpr float PITCH_INPUT_SCALE = 2.0f;
#else
static constexpr float YAW_INPUT_SCALE = 10.0f;
static constexpr float PITCH_INPUT_SCALE = 5.0f;
#endif

static constexpr float MOUSE_SENS_YAW = 0.1f;
static constexpr float MOUSE_SENS_PITCH = 0.1f;

// Motor constants --------------------------------

constexpr CanBus CAN_WHEELS = CanBus::CAN_BUS1;
constexpr CanBus CAN_TURRET = CanBus::CAN_BUS1;
constexpr CanBus CAN_SHOOTER = CanBus::CAN_BUS2;

// chassis
const MotorConfig WHEEL_LF{M3508, MOTOR2, CAN_WHEELS, true, "left front wheel", PID_WHEELS, {}};
const MotorConfig WHEEL_RF{M3508, MOTOR1, CAN_WHEELS, false, "right front wheel", PID_WHEELS, {}};
const MotorConfig WHEEL_LB{M3508, MOTOR3, CAN_WHEELS, true, "left back wheel", PID_WHEELS, {}};
const MotorConfig WHEEL_RB{M3508, MOTOR4, CAN_WHEELS, false, "right back wheel", PID_WHEELS, {}};

// flywheels
const MotorConfig
    FLYWHEEL_L{M3508_NOGEARBOX, MOTOR3, CAN_SHOOTER, true, "flywheel left", PID_FLYWHEEL, {}};
const MotorConfig
    FLYWHEEL_R{M3508_NOGEARBOX, MOTOR4, CAN_SHOOTER, false, "flywheel right", PID_FLYWHEEL, {}};

// agitator
const MotorConfig AGITATOR{M3508, MOTOR1, CAN_SHOOTER, false, "agitator", PID_AGITATOR, {}};

// turret
const MotorConfig YAW_L{M3508, MOTOR5, CAN_TURRET, false, "yaw left", PID_VELOCITY_DEFAULT, {}};
const MotorConfig YAW_R{M3508, MOTOR6, CAN_TURRET, false, "yaw right", PID_VELOCITY_DEFAULT, {}};
const MotorConfig PITCH{GM6020, MOTOR7, CAN_TURRET, false, "pitch", PID_VELOCITY_DEFAULT, {}};

// Velocities -------------------------------------

#ifdef MODE_DEMO
static constexpr float WHEEL_MAX_VEL = 10.0f;  // rad/s
#else
static constexpr float WHEEL_MAX_VEL = 50.0f;  // rad/s
#endif

static constexpr float MAX_LINEAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS;               // m/s
static constexpr float MAX_ANGULAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS / WHEEL_LXY;  // rad/s

const float TARGET_PROJECTILE_VELOCITY = 30;  // m/s
const float FLYWHEEL_SPEED = 160.0f;

const float BALLS_PER_SEC = 20.0f;
const float BALLS_PER_REV = 18.0f;

const float JAM_TRIGGER_RATIO = 0.2;      // measured speed to driven speed ratio
const float JAM_TRIGGER_DURATION = 0.1f;  // s
const float UNJAM_DURATION = 0.1f;        // s
const float UNJAM_SPEED = 30.0f;          // rev/s

// Heat Buffers -------------------------------------

const uint16_t BARREL_HEAT_BUFFER = 50.0f;
