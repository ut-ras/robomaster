/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of taproot-examples.
 *
 * taproot-examples is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * taproot-examples is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with taproot-examples.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * This is part of aruw's library.
 *
 * This is example code for running friction wheels. As you can
 * see, there is a generic update pid loop that is independent of
 * what command is given to the subsystem. Additionally, the
 * subsystem contains variables specific to the subsystem
 * (pid controllers, motors, etc). If a control loop is specific
 * to a command, it should NOT be in a subsystem. For example,
 * control code to pulse the friction wheels should be located
 * outside of this class because pulsing is a specific command.
 */

#ifndef EXAMPLE_SUBSYSTEM_HPP_
#define EXAMPLE_SUBSYSTEM_HPP_

#include "tap/control/command_scheduler.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/math/filter/pid.hpp"

class Drivers;

class ChassisSubsystem : public tap::control::Subsystem
{
public:
    ChassisSubsystem(
        tap::Drivers* drivers,
        tap::motor::DjiMotor* yawMotor,
        tap::motor::MotorId leftFrontMotorId = tap::motor::MOTOR1,
        tap::motor::MotorId rightFrontMotorId = tap::motor::MOTOR2,
        tap::motor::MotorId leftBackMotorId = tap::motor::MOTOR3,
        tap::motor::MotorId rightBackMotorId = tap::motor::MOTOR4);

    void initialize() override;

    void setDesiredRpm(float desRpm);

    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Chassis subsystem"; }

private:
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    static constexpr float PID_P = 5.0f;
    static constexpr float PID_I = 0.0f;
    static constexpr float PID_D = 1.0f;
    static constexpr float PID_MAX_ERROR_SUM = 0.0f;
    static constexpr float PID_MAX_OUTPUT = 16000;

    modm::Pid<float> velocityPidLeftFrontWheel;
    modm::Pid<float> velocityPidLeftBackWheel;
    modm::Pid<float> velocityPidRightFrontWheel;
    modm::Pid<float> velocityPidRightBackWheel;

    float desiredWheelRPM[4];
    float maxRPM = 8000;  // assume 8000 to be max rpm
    modm::Pid<float> pid[4];
    float startYaw;
    bool imuDrive;

    const tap::motor::DjiMotor* yawMotor;

    bool setStartTurret;
    float startTurretLoc;

    bool slowMode;
    float slowFactor;

    float energyBuffer;

    void ChassisSubsystem::setDesiredOutput(float x, float y, float r);
    void updateMotorRpmPid(
        modm::Pid<float>* pid,
        tap::motor::DjiMotor* const motor,
        float desiredRpm);
    void ChassisSubsystem::refresh();
    void ChassisSubsystem::updateMotorRpmPid(
        modm::Pid<float>* pid,
        tap::motor::DjiMotor* motor,
        float desiredRpm);

#if defined(PLATFORM_HOSTED) && defined(ENV_UNIT_TESTS)
public:
    tap::mock::DjiMotorMock leftWheel;
    tap::mock::DjiMotorMock rightWheel;

private:
#else
    tap::motor::DjiMotor leftFrontWheel;
    tap::motor::DjiMotor leftBackWheel;
    tap::motor::DjiMotor rightFrontWheel;
    tap::motor::DjiMotor rightBackWheel;
#endif
};

#endif  // EXAMPLE_SUBSYSTEM_HPP_
