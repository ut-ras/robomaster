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
}

void ChassisSubsystem::refresh() 
{
    updateMotorRpmPID(&pid, &rightFrontMotor, desiredWheelRPM[0]);
    updateMotorRpmPID(&pid, &leftFrontMotor, desiredWheelRPM[1]);
    updateMotorRpmPID(&pid, &leftBackMotor, desiredWheelRPM[2]);
    updateMotorRpmPID(&pid, &rightBackMotor, desiredWheelRPM[3]);
}

void ChassisSubsystem::updateMotorRpmPID(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm)
{
    pid->update(desiredRpm - motor->getShaftRPM());
    motor->setDesiredOutput(pid->getValue());
}

void ChassisSubsystem::setDesiredOutput(float x, float y, float r)
{
    vector.setX(x);
    vector.setY(y);

    float theta = vector.getAngle();
    float power = vector.getLength();

    float sin = sinf(theta - M_PI_4);
    float cos = cosf(theta - M_PI_4);
    float max = modm::max(std::abs(sin), std::abs(cos));

    desiredWheelRPM[0] = power * sin/max + r;   // right front wheel
    desiredWheelRPM[1] = -power * cos/max + r;   // left front wheel
    desiredWheelRPM[2] = -power * sin/max + r;   // left back wheel
    desiredWheelRPM[3] = power * cos/max + r;   // right back wheel

    if ((power + abs(r)) > 1)
    {
        desiredWheelRPM[0] /= power - r;   // right front wheel
        desiredWheelRPM[1] /= power + r;   // left front wheel
        desiredWheelRPM[2] /= power + r;   // left back wheel
        desiredWheelRPM[3] /= power - r;   // right back wheel
    }

    for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredWheelRPM); i++)
    {
        desiredWheelRPM[i] *= maxRPM;
    }
}
}  // namespace chassis
}  // namespace control
