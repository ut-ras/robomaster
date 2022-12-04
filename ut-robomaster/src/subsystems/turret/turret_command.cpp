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
#include "turret_subsystem.hpp"

#define POSDEADZONE 0.2

using tap::control::Subsystem;

TurretCommand::TurretCommand(tap::Drivers* drivers, TurretSubsystem* subsystem)
    : drivers(drivers), 
    subsystem(subsystem)
{
    addSubsystemRequirement(subsystem);
}

void TurretCommand::initialize() {subsystem->setDesiredRpm(0, 0);}

void TurretCommand::execute() { 
    float x = drivers->remote.getChannel(tap::communication::serial::Remote::Channel::RIGHT_HORIZONTAL);
    float y = -(drivers->remote.getChannel(tap::communication::serial::Remote::Channel::RIGHT_VERTICAL));

    if (fabs(x) > POSDEADZONE || fabs(y) > POSDEADZONE){
        subsystem->setDesiredRpm(x * DEFAULT_WHEEL_RPM, y * DEFAULT_WHEEL_RPM);
    } else{
        subsystem->setDesiredRpm(0.0f, 0.0f);
    }

    subsystem->setDesiredRpm(x * DEFAULT_WHEEL_RPM, y * DEFAULT_WHEEL_RPM);
}

void TurretCommand::end(bool interrupted)
{
    subsystem->setDesiredRpm(0, 0);
}

bool TurretCommand::isFinished(void) const { return false; }
