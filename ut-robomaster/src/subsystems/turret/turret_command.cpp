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

#include "turret_command.hpp"

#define ANALOG_DEAD_ZONE 0.1

namespace subsystems
{
namespace turret
{
void TurretCommand::initialize() {}

void TurretCommand::execute()
{
    Remote* remote = &drivers->remote;
    float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
    float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);

    if (abs(h) < ANALOG_DEAD_ZONE) h = 0.0f;
    if (abs(v) < ANALOG_DEAD_ZONE) v = 0.0f;

    float yawInput = h * abs(h);    // quadratic input map
    float pitchInput = v * abs(v);  // quadratic input map

    yaw -= yawInput * yawInputScale;
    pitch += pitchInput * pitchInputScale;
    pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);

    subsystem->inputManualAngles(yaw, pitch);
}

void TurretCommand::end(bool) {}

bool TurretCommand::isFinished(void) const { return false; }
}  // namespace turret
}  // namespace subsystems
