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
#include "turret_subsystem.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/communication/serial/remote.hpp"
#include "modm/math/geometry/vector.hpp"

#include "tap/drivers.hpp"

using namespace tap;
using namespace tap::algorithms;

namespace control
{
namespace turret
{
void TurretSubsystem::initialize()
{
    yawMotor.initialize();
    pitchMotor.initialize();

    startYaw = yawMotor.getEncoderUnwrapped();
    yawSetValue = startYaw;
    yawIsSet = true;

    startPitch = pitchMotor.getEncoderUnwrapped();
    pitchSetValue = startPitch;
    pitchIsSet = true;
}

void TurretSubsystem::refresh() 
{
    // NOTE: Motors are switched with each other, needs investigating
    updateMotorRpmPID(&pid, &yawMotor, desiredRPM[0]);
    updateMotorRpmPID(&pid, &pitchMotor, desiredRPM[1]);
}

void TurretSubsystem::updateMotorRpmPID(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm)
{
    pid->update(desiredRpm - motor->getShaftRPM());
    motor->setDesiredOutput(pid->getValue());
}

void TurretSubsystem::setDesiredOutput(float x, float y)
{
    if (x != 0.0f) {
        desiredRPM[0] = x * 4;   // yaw motor
        yawIsSet = false;
    }

    else {
        if (!yawIsSet) {
            yawSetValue = yawMotor.getEncoderUnwrapped();
            yawIsSet = true;
        }

        pid.update(yawSetValue - yawMotor.getEncoderUnwrapped());
        desiredRPM[0] = pid.getValue();
    }

    if (y != 0.0f) {
        if ((y > 0 && pitchMotor.getEncoderUnwrapped() >= startPitch) || (y < 0 && pitchMotor.getEncoderUnwrapped() <= startPitch - PITCH_RANGE)) {
            desiredRPM[1] = 0.0f;   // pitch motor
        }
        else {
            desiredRPM[1] = y * 16;   // pitch motor
        }
    }
}
}  // namespace turret
}  // namespace control
