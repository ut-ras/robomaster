/*
 * Copyright (c) 2021-2022 UT Robomaster
 *
 * This file is part of ut-robomaster.
 *
 * ut-robomaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ut-robomaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ut-robomaster.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "chassis_subsystem.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/communication/serial/remote.hpp"
#include "modm/math/geometry/vector.hpp"

#include "tap/drivers.hpp"

using namespace tap;
using namespace tap::algorithms;

namespace control
{
namespace chassis
{
void ChassisSubsystem::initialize()
{
    leftFrontMotor.initialize();
    leftBackMotor.initialize();
    rightFrontMotor.initialize();
    rightBackMotor.initialize();
    drivers->bmi088.requestRecalibration();
    startYaw = drivers->bmi088.getYaw();
    imuDrive = true;

    setStartTurret = false;
    startTurretLoc = 0.0f;

    slowFactor = 1.0f;
}

void ChassisSubsystem::refresh() 
{
    updateMotorRpmPID(&pid[0], &rightFrontMotor, desiredWheelRPM[0]);
    updateMotorRpmPID(&pid[1], &leftFrontMotor, desiredWheelRPM[1]);
    updateMotorRpmPID(&pid[2], &leftBackMotor, desiredWheelRPM[2]);
    updateMotorRpmPID(&pid[3], &rightBackMotor, desiredWheelRPM[3]);

    // from aruw-mcb chassis_subsystem.cpp
    float powerScalar = powerLimiter();
    if (compareFloatClose(1.0f, powerScalar, 1E-3))
    {
        return;
    }

    // float totalError = 0.0f;
    // for (size_t i = 0; i < 4; i++)
    // {
    //     totalError += abs(pid[i].getLastError());
    // }

    // bool totalErrorZero = compareFloatClose(0.0f, totalError, 1E-3);
    for (size_t i = 0; i < 4; i++)
    {
        // float velocityErrorScalar = totalErrorZero ? (1.0f / 4) : (abs(pid[i].getLastError()) / totalError);
        // float modifiedPowerScalar =
        //     limitVal(4 * powerScalar * velocityErrorScalar, 0.0f, 1.0f);
        motors[i]->setDesiredOutput(motors[i]->getOutputDesired() * powerScalar);
    }
}

void ChassisSubsystem::updateMotorRpmPID(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm)
{
    if (drivers->remote.keyPressed(tap::communication::serial::Remote::Key::SHIFT)){
        slowFactor = 0.5f;
    }
    else { 
        slowFactor = 1.0f; 
    }

    pid->update((desiredRpm * slowFactor) - motor->getShaftRPM());
    motor->setDesiredOutput(pid->getValue());
}
///@brief 

///
void ChassisSubsystem::setDesiredOutput(float x, float y, float r)
{
    if (drivers->bmi088.getImuState() == tap::communication::sensors::imu::ImuInterface::ImuState::IMU_CALIBRATING) //if the 6020 is set to the calibrated value, set all RPMs to 0
    {
        for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredWheelRPM); i++)
        {
            desiredWheelRPM[i] = 0;
        }
        return;
    }
    
    vector.setX(x);
    vector.setY(y);

    // if (!imuDrive && drivers->remote.getSwitch(tap::communication::serial::Remote::Switch::LEFT_SWITCH) == tap::communication::serial::Remote::SwitchState::DOWN)
    // {       //TODO: Fix the IMU and the if statement to not use remote commands
    //     imuDrive = true;  
    //     drivers->bmi088.requestRecalibration();
    //     startYaw = drivers->bmi088.getYaw();
    // }

    // else if (drivers->remote.getSwitch(tap::communication::serial::Remote::Switch::LEFT_SWITCH) == tap::communication::serial::Remote::SwitchState::MID)
    // {
    //     imuDrive = false;
    // }

    // if (imuDrive && drivers->bmi088.getImuState() == tap::communication::sensors::imu::ImuInterface::ImuState::IMU_CALIBRATED)
    // {
    //     float offset = modm::toRadian(drivers->bmi088.getYaw() - startYaw);
    //     vector.rotate(offset);
    // }
    if (imuDrive && drivers->bmi088.getImuState() == tap::communication::sensors::imu::ImuInterface::ImuState::IMU_CALIBRATED)
    {
        if (!setStartTurret && yawMotor->isMotorOnline()){
            startTurretLoc = yawMotor->getEncoderUnwrapped();
            setStartTurret = true;
        }
        float turretOffset = modm::toRadian((yawMotor->getEncoderUnwrapped() - startTurretLoc) * 360 / 8192);
        vector.rotate(turretOffset);
    }
    
    float theta = vector.getAngle();
    float power = vector.getLength();

    float sin = sinf(theta - M_PI_4);
    float cos = cosf(theta - M_PI_4);
    float max = modm::max(std::abs(sin), std::abs(cos));

    desiredWheelRPM[0] = power * cos/max + r;   // right front wheel
    desiredWheelRPM[1] = power * sin/max - r;   // left front wheel
    desiredWheelRPM[2] = power * cos/max - r;   // left back wheel
    desiredWheelRPM[3] = power * sin/max + r;   // right back wheel

    if ((power + abs(r)) > 1)
    {
        desiredWheelRPM[0] /= power + abs(r);   // right front wheel
        desiredWheelRPM[1] /= power + abs(r);   // left front wheel
        desiredWheelRPM[2] /= power + abs(r);   // left back wheel
        desiredWheelRPM[3] /= power + abs(r);   // right back wheel
    }

    for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredWheelRPM); i++)
    {
        desiredWheelRPM[i] *= maxRPM;
    }
}

float ChassisSubsystem::powerLimiter()
{
    if (!drivers->refSerial.getRefSerialReceivingData())
    {
        drivers->leds.set(tap::gpio::Leds::Green, true);
        return 1.0f;
    }

    energyBuffer = drivers->refSerial.getRobotData().chassis.powerBuffer;
    if (energyBuffer < ENERGY_BUFFER_LIMIT_THRESHOLD)
    {
        return limitVal(
            static_cast<float>(energyBuffer - ENERGY_BUFFER_CRIT_THRESHOLD) /
                ENERGY_BUFFER_LIMIT_THRESHOLD,
            0.0f,
            1.0f);
    }
    else
    {
        return 1.0f;
    }
}
}  // namespace chassis
}  // namespace control
