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

#define POSDEADZONE 0.2

namespace subsystems
{
namespace turret
{
void TurretCommand::initialize() { subsystem->setDesiredRpm(0, 0); }

void TurretCommand::execute()
{
    Remote* remote = &drivers->remote;
    // float x = static_cast<float>(remote->getMouseX()) / 1.0f;
    // float y = static_cast<float>(remote->getMouseY()) / 1.0f;
    float x = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL)+(Remote->getMouseX()/1.0f);
    float y = remote->getChannel(Remote::Channel::LEFT_VERTICAL)+(Remote->getMouseY()/1.0f);
    subsystem->setDesiredRpm(-1 * x * 100.0f, y * 100.0f);
}

void TurretCommand::end(bool interrupted) { subsystem->setDesiredRpm(0, 0); }

bool TurretCommand::isFinished(void) const { return false; }
}  // namespace turret
}  // namespace subsystems
