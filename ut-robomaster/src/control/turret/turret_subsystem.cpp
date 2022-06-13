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
    startYaw = yawMotor.getEncoderWrapped();
    startPitch = pitchMotor.getEncoderUnwrapped();
}

void TurretSubsystem::refresh() 
{
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
    desiredRPM[0] = (x / (MOUSE_SCALAR * INT16_MAX)) * maxRPM;   // yaw motor
    if ((y < 0 && pitchMotor.getEncoderUnwrapped() <= startPitch) || (y > 0 && pitchMotor.getEncoderUnwrapped() >= startPitch + PITCH_RANGE)) {
        desiredRPM[1] = 0.0f;
    }
    else {
        desiredRPM[1] = (y / (MOUSE_SCALAR * INT16_MAX)) * maxRPM;   // pitch motor
    }
}
}  // namespace turret
}  // namespace control
