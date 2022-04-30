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
#include "gimbal_subsystem.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/communication/serial/remote.hpp"
#include "modm/math/geometry/vector.hpp"

#include "tap/drivers.hpp"

using namespace tap;
using namespace tap::algorithms;

namespace control
{
namespace gimbal
{
void GimbalSubsystem::initialize()
{
    yawMotor.initialize();
    pitchMotor.initialize();
    startYaw = yawMotor.getEncoderWrapped();
    startPitch = pitchMotor.getEncoderUnwrapped();
}

void GimbalSubsystem::refresh() 
{
    // updateMotorRpmPID(&pid, &yawMotor, desiredRPM[0]);
    yawMotor.setDesiredOutput(10000.0f);
    updateMotorRpmPID(&pid, &pitchMotor, desiredRPM[1]);
}

void GimbalSubsystem::updateMotorRpmPID(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm)
{
    pid->update(desiredRpm - motor->getShaftRPM());
    motor->setDesiredOutput(pid->getValue());
}

void GimbalSubsystem::setDesiredOutput(float x, float y)
{
    desiredRPM[0] = x * 320.0f;   // yaw motor
    desiredRPM[1] = y * maxRPM;   // pitch motor
}
}  // namespace gimbal
}  // namespace control
