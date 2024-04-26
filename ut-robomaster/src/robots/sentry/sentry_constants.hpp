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
// Kinematic constants ---------------------------------------------

// chassis ------------
static constexpr int WHEELS = 4;
static constexpr float WHEEL_DISTANCE_X = 0.391f;  // meters
static constexpr float WHEEL_DISTANCE_Y = 0.315f;  // meters
static constexpr float WHEEL_RADIUS = 0.1524f;     // meters
static constexpr float WHEEL_LXY = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;

// flywheels ---------
static constexpr int FLYWHEELS = 4;

// turret --------
constexpr float PITCH_MIN = -0.2185f;         // rad
constexpr float PITCH_MAX = 0.2299f;          // rad
constexpr float CAMERA_TO_PITCH = 0.13555f;   // distance from main camera lens to pitch axis (m)
constexpr float NOZZLE_TO_PITCH = 0.18151f;   // distance from barrel nozzle to pitch axis (m)
constexpr float CAMERA_TO_BARRELS = 0.0427f;  // vertical ctc offset from camera lens to barrel (m)
constexpr float CAMERA_X_OFFSET = -0.0335f;   // horizontal offset of main camera lens (m)

static constexpr float YAW_REDUCTION = 2.0f;
static constexpr float PITCH_REDUCTION = 4.0f;

// Tuning constants -----------------------------------------------
constexpr PidConstants PID_VELOCITY_DEFAULT = {0.75f, 12.0f, 0.0f};

constexpr PidConstants PID_WHEELS = PID_VELOCITY_DEFAULT;

constexpr PidConstants PID_FLYWHEEL = {0.1f, 0.1f, 0.0f};

constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;

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
    .maxOutput = 32'000.0f,
    .tQDerivativeKalman = 1.0f,
    .tRDerivativeKalman = 30.0f,
    .tQProportionalKalman = 1.0f,
    .tRProportionalKalman = 0.0f,
    .errDeadzone = 0.0f,
    .errorDerivativeFloor = 0.0f,
};

// Input constants ----------------------------------------
#ifdef DEMO_MODE
static constexpr float YAW_INPUT_SCALE = 1.0f;
static constexpr float PITCH_INPUT_SCALE = 1.0f;

#else
static constexpr float YAW_INPUT_SCALE = 10.0f;
static constexpr float PITCH_INPUT_SCALE = 5.0f;
#endif

static constexpr float MOUSE_SENS_YAW = 0.1f;
static constexpr float MOUSE_SENS_PITCH = 0.1f;

// Motor constants -------------------------------------

// Chassis
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

// Flywheels
constexpr MotorId ID_FLYWHEEL_TL = MOTOR3;
constexpr MotorId ID_FLYWHEEL_TR = MOTOR4;
constexpr MotorId ID_FLYWHEEL_BL = MOTOR5;
constexpr MotorId ID_FLYWHEEL_BR = MOTOR6;

const bool TL_INVERTED = true;
const bool TR_INVERTED = false;
const bool BL_INVERTED = false;
const bool BR_INVERTED = true;

const char* const TL_NAME = "flywheel top left";
const char* const TR_NAME = "flywheel top right";
const char* const BL_NAME = "flywheel bottom left";
const char* const BR_NAME = "flywheel bottom right";

const Motor TL{M3508_NOGEARBOX, ID_FLYWHEEL_TL, CAN_SHOOTER, TL_INVERTED, TL_NAME, PID_FLYWHEEL};
const Motor TR{M3508_NOGEARBOX, ID_FLYWHEEL_TR, CAN_SHOOTER, TR_INVERTED, TR_NAME, PID_FLYWHEEL};
const Motor BL{M3508_NOGEARBOX, ID_FLYWHEEL_BL, CAN_SHOOTER, BL_INVERTED, BL_NAME, PID_FLYWHEEL};
const Motor BR{M3508_NOGEARBOX, ID_FLYWHEEL_BR, CAN_SHOOTER, BR_INVERTED, BR_NAME, PID_FLYWHEEL};
// agitator

constexpr MotorId ID_AGITATOR_L = MOTOR1;
constexpr MotorId ID_AGITATOR_R = MOTOR2;

const bool AGITATOR_L_INVERTED = false;
const bool AGITATOR_R_INVERTED = true;

const char* const AGITATOR_L_NAME = "agitator left";
const char* const AGITATOR_R_NAME = "agitator right";

const Motor AGITATOR_L{
    M2006,
    ID_AGITATOR_L,
    CAN_SHOOTER,
    AGITATOR_L_INVERTED,
    AGITATOR_L_NAME,
    PID_AGITATOR};

const Motor AGITATOR_R{
    M2006,
    ID_AGITATOR_R,
    CAN_SHOOTER,
    AGITATOR_R_INVERTED,
    AGITATOR_R_NAME,
    PID_AGITATOR};

// turret (to be done)
constexpr MotorId ID_YAW = MOTOR6;
constexpr MotorId ID_PITCH = MOTOR7;

const bool YAW_INVERTED = false;
const bool PITCH_INVERTED = false;

const char* const YAW_NAME = "yaw";
const char* const PITCH_NAME = "pitch";

// Velocities -------------------------------------

#ifdef MODE_DEMO
static constexpr float WHEEL_MAX_VEL = 10.0f;  // rad/s
#else
static constexpr float WHEEL_MAX_VEL = 50.0f;  // rad/s
#endif

static constexpr float MAX_LINEAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS;               // m/s
static constexpr float MAX_ANGULAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS / WHEEL_LXY;  // rad/s

static constexpr modm::Pair<uint16_t, float> FLYWHEEL_RPS_MAPPING[] = {
    {15, 67.0f},
    {18, 75.0f},
    {30, 122.0f}};

static constexpr float DEFAULT_SPEED = 70.0f;  // default speed for flywheels

const float BALLS_PER_SEC = 10.0f;
const float BALLS_PER_REV = 8.0f;

const float JAM_TRIGGER_RATIO = 0.5;      // measured speed to driven speed ratio
const float JAM_TRIGGER_DURATION = 0.1f;  // s
const float UNJAM_DURATION = 0.1f;        // s
const float UNJAM_SPEED = 15.0f;          // rev/s

// Heat Buffers -------------------------------------

const uint16_t BARREL_HEAT_BUFFER = 20.0f;
