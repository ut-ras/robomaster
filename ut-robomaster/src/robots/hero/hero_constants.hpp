#pragma once

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/container/pair.hpp"
#include "subsystems/sound/sounds/sound_smb_powerup.hpp"
#include "utils/motor_controller/motor_constants.hpp"
#include "utils/motor_controller/pid.hpp"

using motor_controller::PidConstants;
using tap::can::CanBus;
using namespace tap::motor;
using namespace motor_controller;

// General constants ------------------------------------------------
constexpr CanBus CAN_WHEELS = CanBus::CAN_BUS1;
constexpr CanBus CAN_TURRET = CanBus::CAN_BUS1;
constexpr CanBus CAN_SHOOTER = CanBus::CAN_BUS2;

constexpr bool USE_BALLISTICS = false;
constexpr int BALLISTIC_ITERATIONS = 2;

const Sound SOUND_STARTUP = SOUND_SMB_POWERUP;

// Kinematics ------------------------------------------------------

// chassis

static constexpr int WHEELS = 4;
static constexpr float WHEEL_DISTANCE_X = 0.525f;  // meters
static constexpr float WHEEL_DISTANCE_Y = 0.400f;  // meters
static constexpr float WHEEL_RADIUS = 0.1524f;     // meters
static constexpr float WHEEL_LXY = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;

// flywheel

static constexpr int FLYWHEELS = 2;

// turret

// NEED TO MEASURE (MAYBE ADD NEW CONSTANTS)
constexpr float CAMERA_TO_PITCH = 0.0f;    // distance from main camera lens to pitch axis (m)
constexpr float NOZZLE_TO_PITCH = 0.0f;    // distance from barrel nozzle to pitch axis (m)
constexpr float CAMERA_TO_BARRELS = 0.0f;  // vertical ctc offset from camera lens to barrel (m)
constexpr float CAMERA_X_OFFSET = 0.0f;    // horizontal offset of main camera lens (m)
constexpr float PITCH_MIN = -0.3349f;      // rad
constexpr float PITCH_MAX = 0.3534f;       // rad

static constexpr float YAW_REDUCTION = 2.0f;
static constexpr float PITCH_REDUCTION = 1.0f;

// Tuning Constants -----------------------------------

// PID constants
constexpr PidConstants PID_VELOCITY_DEFAULT = {0.75f, 12.0f, 0.0f};
constexpr PidConstants PID_WHEELS = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_VELOCITY = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_POSITION = {0.1f, 0.1f, 0.0f};

constexpr PidConstants PID_FLYWHEEL = {0.1f, 0.1f, 0.0f};
constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_FEEDER = PID_VELOCITY_DEFAULT;

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

// Input Constants ---------------------------------------

#ifdef DEMO_MDOE
static constexpr float YAW_INPUT_SCALE = 1.0f;
static constexpr float PITCH_INPUT_SCALE = 1.0f;
#else
static constexpr float YAW_INPUT_SCALE = 4.0f;
static constexpr float PITCH_INPUT_SCALE = 5.0f;
#endif

static constexpr float MOUSE_SENS_YAW = 0.0045f;
static constexpr float MOUSE_SENS_PITCH = 0.002f;

// Motor Constants -------------------------------------

// chassis

constexpr MotorId ID_WHEEL_LF = MOTOR2;
constexpr MotorId ID_WHEEL_RF = MOTOR1;
constexpr MotorId ID_WHEEL_LB = MOTOR3;
constexpr MotorId ID_WHEEL_RB = MOTOR4;

const bool LF_INVERTED = true;
const bool RF_INVERTED = false;
const bool LB_INVERTED = true;
const bool RB_INVERTED = false;

const char* const LF_NAME = "left front";
const char* const RF_NAME = "right front";
const char* const LB_NAME = "left back";
const char* const RB_NAME = "right back";

const Motor LF{M3508, ID_WHEEL_LF, CAN_WHEELS, LF_INVERTED, LF_NAME, PID_WHEELS};
const Motor RF{M3508, ID_WHEEL_RF, CAN_WHEELS, RF_INVERTED, RF_NAME, PID_WHEELS};
const Motor LB{M3508, ID_WHEEL_LB, CAN_WHEELS, LB_INVERTED, LB_NAME, PID_WHEELS};
const Motor RB{M3508, ID_WHEEL_RB, CAN_WHEELS, RB_INVERTED, RB_NAME, PID_WHEELS};

// flywheels

constexpr MotorId ID_FLYWHEEL_L = MOTOR3;
constexpr MotorId ID_FLYWHEEL_R = MOTOR4;

const Motor FL{M3508_NOGEARBOX, ID_FLYWHEEL_L, CAN_SHOOTER, true, "flywheel left", PID_FLYWHEEL};
const Motor FR{M3508_NOGEARBOX, ID_FLYWHEEL_R, CAN_SHOOTER, false, "flywheel right", PID_FLYWHEEL};

// agitator

constexpr MotorId ID_AGITATOR = MOTOR1;
constexpr MotorId ID_FEEDER = MOTOR2;

const Motor AGITATOR{M3508, ID_AGITATOR, CAN_SHOOTER, false, "agitator", PID_AGITATOR};
const Motor FEEDER{M2006, ID_FEEDER, CAN_SHOOTER, false, "feeder", PID_FEEDER};
// turret (to be done)

constexpr MotorId ID_YAW = MOTOR5;
constexpr MotorId ID_PITCH = MOTOR6;

// Velocities ----------------------------

#ifdef MODE_DEMO
static constexpr float WHEEL_MAX_VEL = 10.0f;  // rad/s
#else
static constexpr float WHEEL_MAX_VEL = 120.0f;  // rad/s
#endif

static constexpr float MAX_LINEAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS;               // m/s
static constexpr float MAX_ANGULAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS / WHEEL_LXY;  // rad/s

static constexpr float DEFAULT_SPEED = 60.0f;  // default speed for the flywheels

static constexpr modm::Pair<uint16_t, float> FLYWHEEL_RPS_MAPPING[] = {{10, 105.0f}, {16, 190.0f}};

const float BALLS_PER_SEC = 4.0f;
const float BALLS_PER_REV = 6.0f;
const float FEEDER_RATIO = 1.0f;  // feeder speed / agitator speed

const float JAM_TRIGGER_RATIO = 0.5;      // measured speed to driven speed ratio
const float JAM_TRIGGER_DURATION = 0.1f;  // s
const float UNJAM_DURATION = 0.1f;        // s
const float UNJAM_SPEED = 12.0f;          // rev/s

// Heat Buffers ---------------------------

const uint16_t BARREL_HEAT_BUFFER = 100.0f;
