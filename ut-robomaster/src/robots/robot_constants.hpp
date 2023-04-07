#ifndef ROBOT_CONSTANTS_HPP_
#define ROBOT_CONSTANTS_HPP_

#include "tap/communication/can/can_bus.hpp"
#include "tap/motor/dji_motor.hpp"

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

constexpr MotorId ID_YAW = MOTOR6;
constexpr MotorId ID_PITCH = MOTOR7;

constexpr PidConstants PID_VELOCITY_DEFAULT = {0.75f, 12.0f, 0.0f};
constexpr PidConstants PID_WHEELS = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_VELOCITY = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_TURRET_POSITION = {0.1f, 0.1f, 0.0f};

#if defined TARGET_STANDARD || defined TARGET_SENTRY
constexpr MotorId ID_FLYWHEEL_TL = MOTOR3;
constexpr MotorId ID_FLYWHEEL_TR = MOTOR4;
constexpr MotorId ID_FLYWHEEL_BL = MOTOR5;
constexpr MotorId ID_FLYWHEEL_BR = MOTOR6;

constexpr MotorId ID_AGITATOR_L = MOTOR1;
constexpr MotorId ID_AGITATOR_R = MOTOR2;

constexpr PidConstants PID_FLYWHEEL = {0.1f, 0.1f, 0.0f};
constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;
#elif defined TARGET_HERO
constexpr MotorId ID_FLYWHEEL_L = MOTOR3;
constexpr MotorId ID_FLYWHEEL_R = MOTOR4;
constexpr MotorId ID_AGITATOR = MOTOR1;
constexpr MotorId ID_FEEDER = MOTOR2;

constexpr PidConstants PID_FLYWHEEL = {0.1f, 0.1f, 0.0f};
constexpr PidConstants PID_AGITATOR = PID_VELOCITY_DEFAULT;
constexpr PidConstants PID_FEEDER = PID_VELOCITY_DEFAULT;
#endif

#endif