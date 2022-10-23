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

#include "chassis_subsystem.hpp"

#include "tap/control/chassis/chassis_subsystem_interface.hpp"

#include "modm/math/geometry.hpp"

#include "drivers.hpp"

ChassisSubsystem::ChassisSubsystem(
    tap::Drivers* drivers,
    tap::motor::DjiMotor* yawMotor,
    tap::motor::MotorId leftFrontMotorId,
    tap::motor::MotorId rightBackMotorId,
    tap::motor::MotorId rightFrontMotorId,
    tap::motor::MotorId leftBackMotorId)
    : tap::control::Subsystem(drivers),
      desiredWheelRPM({0.0f, 0.0f, 0.0f, 0.0f}),
      leftFrontWheel(drivers, leftFrontMotorId, CAN_BUS_MOTORS, true, "left front motor"),
      leftBackWheel(drivers, leftBackMotorId, CAN_BUS_MOTORS, true, "left back motor"),
      rightFrontWheel(drivers, rightFrontMotorId, CAN_BUS_MOTORS, true, "right front motor"),
      rightBackWheel(drivers, rightBackMotorId, CAN_BUS_MOTORS, true, "right back motor"),
      yawMotor(yawMotor),
      pid{modm::Pid<float>(PID_P, PID_I, PID_D, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_P, PID_I, PID_D, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_P, PID_I, PID_D, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_P, PID_I, PID_D, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT)},
      imuDrive(true),
      setStartTurret(false),
      startTurretLoc(0.0f),
      slowFactor(1.0f)
{
}

void ChassisSubsystem::initialize()
{
    leftFrontWheel.initialize();
    leftBackWheel.initialize();
    rightFrontWheel.initialize();
    rightBackWheel.initialize();
}

void ChassisSubsystem::setDesiredOutput(float x, float y, float r)
{
    if (drivers->bmi088.getImuState() ==
        tap::communication::sensors::imu::ImuInterface::ImuState::
            IMU_CALIBRATING)  // if the 6020 is set to the calibrated value, set all RPMs to 0
    {
        for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredWheelRPM); i++)
        {
            desiredWheelRPM[i] = 0;
        }
        return;
    }

    modm::Vector2f vector(x, y);

    // if (!imuDrive &&
    // drivers->remote.getSwitch(tap::communication::serial::Remote::Switch::LEFT_SWITCH) ==
    // tap::communication::serial::Remote::SwitchState::DOWN) {       //TODO: Fix the IMU and the if
    // statement to not use remote commands
    //     imuDrive = true;
    //     drivers->bmi088.requestRecalibration();
    //     startYaw = drivers->bmi088.getYaw();
    // }

    // else if (drivers->remote.getSwitch(tap::communication::serial::Remote::Switch::LEFT_SWITCH)
    // == tap::communication::serial::Remote::SwitchState::MID)
    // {
    //     imuDrive = false;
    // }

    // if (imuDrive && drivers->bmi088.getImuState() ==
    // tap::communication::sensors::imu::ImuInterface::ImuState::IMU_CALIBRATED)
    // {
    //     float offset = modm::toRadian(drivers->bmi088.getYaw() - startYaw);
    //     vector.rotate(offset);
    // }
    if (imuDrive && drivers->bmi088.getImuState() ==
                        tap::communication::sensors::imu::ImuInterface::ImuState::IMU_CALIBRATED)
    {
        if (!setStartTurret && yawMotor->isMotorOnline())
        {
            startTurretLoc = yawMotor->getEncoderUnwrapped();
            setStartTurret = true;
        }
        float turretOffset =
            modm::toRadian((yawMotor->getEncoderUnwrapped() - startTurretLoc) * 360 / 8192);
        vector.rotate(turretOffset);
    }

    float theta = vector.getAngle();
    float power = vector.getLength();

    float sin = sinf(theta - M_PI_4);
    float cos = cosf(theta - M_PI_4);
    float max = modm::max(std::abs(sin), std::abs(cos));

    desiredWheelRPM[0] = power * cos / max + r;  // right front wheel
    desiredWheelRPM[1] = power * sin / max - r;  // left front wheel
    desiredWheelRPM[2] = power * cos / max - r;  // left back wheel
    desiredWheelRPM[3] = power * sin / max + r;  // right back wheel

    if ((power + abs(r)) > 1)
    {
        desiredWheelRPM[0] /= power + abs(r);  // right front wheel
        desiredWheelRPM[1] /= power + abs(r);  // left front wheel
        desiredWheelRPM[2] /= power + abs(r);  // left back wheel
        desiredWheelRPM[3] /= power + abs(r);  // right back wheel
    }

    for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredWheelRPM); i++)
    {
        desiredWheelRPM[i] *= maxRPM;
    }
}
void ChassisSubsystem::refresh()
{
    updateMotorRpmPid(&velocityPidLeftFrontWheel, &leftFrontWheel, desiredWheelRPM[1]);
    updateMotorRpmPid(&velocityPidLeftBackWheel, &leftBackWheel, desiredWheelRPM[2]);
    updateMotorRpmPid(&velocityPidRightFrontWheel, &rightFrontWheel, desiredWheelRPM[0]);
    updateMotorRpmPid(&velocityPidRightBackWheel, &rightBackWheel, desiredWheelRPM[3]);
}

void ChassisSubsystem::updateMotorRpmPid(
    modm::Pid<float>* pid,
    tap::motor::DjiMotor* motor,
    float desiredRpm)
{
    pid->update(desiredRpm - motor->getShaftRPM());
    motor->setDesiredOutput(static_cast<int32_t>(pid->getValue()));
}

void ChassisSubsystem::runHardwareTests()
{
    // TODO
}
