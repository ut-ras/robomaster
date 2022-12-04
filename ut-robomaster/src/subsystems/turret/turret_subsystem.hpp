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

#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/control/command_scheduler.hpp"
#include "tap/control/subsystem.hpp"

#include "tap/motor/dji_motor.hpp"

#include "modm/math/filter/pid.hpp"

class Drivers;

class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(
        tap::Drivers* drivers,
        tap::motor::MotorId yawMotorID = tap::motor::MOTOR6,
        tap::motor::MotorId pitchMotorID = tap::motor::MOTOR7);

    void initialize() override;

    void setDesiredRpm(float desRpmYaw, float desRpmPitch);
    void setDesiredRpm(float desRpm);
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

private:
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    static constexpr float PID_P = 5.0f;
    static constexpr float PID_I = 0.0f;
    static constexpr float PID_D = 1.0f;
    static constexpr float PID_MAX_ERROR_SUM = 0.0f;
    static constexpr float PID_MAX_OUTPUT = 16000;

    modm::Pid<float> velocityPidYawMotor;

    modm::Pid<float> velocityPidPitchMotor;

    float desiredRpmYaw, desiredRpmPitch;
    float desiredRpm;
    void updateMotorRpmPid(
        modm::Pid<float>* pid,
        tap::motor::DjiMotor* const motor,
        float desiredRpm);

#if defined(PLATFORM_HOSTED) && defined(ENV_UNIT_TESTS)
public:
    tap::mock::DjiMotorMock yawMotor;
    tap::mock::DjiMotorMock pitchMotor;

private:
#else
    tap::motor::DjiMotor yawMotor;
    tap::motor::DjiMotor pitchMotor;
#endif
};

#endif  // TURRET_SUBSYSTEM_HPP_