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

using tap::control::Subsystem;

TurretCommand::TurretCommand(TurretSubsystem* subsystem, int speed)
    : subsystem(subsystem),
      speed(speed)
{
    addSubsystemRequirement(subsystem);
}

void TurretCommand::initialize() {subsystem->setDesiredRpm(0);}

void TurretCommand::execute() { subsystem->setDesiredRpm(speed); }

void TurretCommand::end(bool interrupted)
{
    subsystem->setDesiredRpm(0);
}

bool TurretCommand::isFinished(void) const { return false; }
