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

    // investigate initial value
    startYaw = yawMotor.getEncoderUnwrapped();
    yawSetValue = startYaw + 2048;
    yawIsSet = true;

    startPitch = pitchMotor.getEncoderUnwrapped();
    pitchSetValue = startPitch;
    pitchIsSet = true;

    prevPosition = drivers->bmi088.getYaw();
    // timer.restart(100);
    // ledTest = false;
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

float TurretSubsystem::imuWrap(float offset)
{
    float positive = offset + 360;
    float negative = offset - 360;

    if (fabs(positive) < fabs(negative) && fabs(positive) < fabs(offset)) {
        drivers->leds.set(tap::gpio::Leds::Red, true);
        drivers->leds.set(tap::gpio::Leds::Green, false);
        drivers->leds.set(tap::gpio::Leds::Blue, false);
        return positive;
    }

    else if (fabs(negative) < fabs(positive) && fabs(negative) < fabs(offset)) {
        drivers->leds.set(tap::gpio::Leds::Blue, true);
        drivers->leds.set(tap::gpio::Leds::Green, false);
        drivers->leds.set(tap::gpio::Leds::Red, false);
        return negative;
    }

    else {
        drivers->leds.set(tap::gpio::Leds::Blue, false);
        drivers->leds.set(tap::gpio::Leds::Green, true);
        drivers->leds.set(tap::gpio::Leds::Red, false);
        return offset;
    }
}

/**
///@brief sets the turret RPMs
///@param x, y are input X and y values from input device
*/
void TurretSubsystem::setDesiredOutput(float x, float y)
{
    if (x != 0.0f) {
        offset = drivers->bmi088.getYaw() - prevPosition;
        offset = imuWrap(offset);
        offset = offset/360 * 8192;
        yawEncoderToRPM.update(offset);
        prevPosition = drivers->bmi088.getYaw();

        desiredRPM[0] = (x * 4) + yawEncoderToRPM.getValue();   // yaw motor
        yawIsSet = false;
    }

    else {
        if (!yawIsSet) {
            yawSetValue = yawMotor.getEncoderUnwrapped();
            yawIsSet = true;
        }
        //Timer may or may need to be removed if bugs occur
        // if (timer.execute()) {
            offset = drivers->bmi088.getYaw() - prevPosition;
            offset = imuWrap(offset);
            offset = offset/360.0f * 8192.0f;
            prevPosition = drivers->bmi088.getYaw();
            // timer.restart(100);
            // if (!ledTest) { drivers->leds.set(tap::gpio::Leds::Red, true); ledTest = true; }
            // else { drivers->leds.set(tap::gpio::Leds::Red, false); ledTest = false; }
            yawSetValue += offset;
        // }

        // if (offset >= 20.0f) {drivers->leds.set(tap::gpio::Leds::Red, true);}
        // else{drivers->leds.set(tap::gpio::Leds::Red, false);}

        yawEncoderToRPM.update(yawSetValue - (float)yawMotor.getEncoderUnwrapped());
        desiredRPM[0] = yawEncoderToRPM.getValue();
    }

    if (y != 0.0f) {
        if ((y > 0 && pitchMotor.getEncoderUnwrapped() >= startPitch) || (y < 0 && pitchMotor.getEncoderUnwrapped() <= startPitch - PITCH_RANGE)) {
            desiredRPM[1] = 0.0f;   // pitch motor
        }
        else {
            desiredRPM[1] = y * 16;   // pitch motor
            pitchIsSet = false;
        }
    }

    else {
        if (!pitchIsSet) {
            pitchSetValue = pitchMotor.getEncoderUnwrapped();
            pitchIsSet = true;
        }

        pitchEncoderToRPM.update(pitchSetValue - pitchMotor.getEncoderUnwrapped());
        desiredRPM[1] = pitchEncoderToRPM.getValue();
    }
}
}  // namespace turret
}  // namespace control
