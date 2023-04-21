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
void TurretCommand::initialize() {
    prevTime = tap::arch::clock::getTimeMilliseconds();
    pitch = subsystem->getPitchTurret()->getAngle();
}

void TurretCommand::execute()
{
    uint32_t currTime = tap::arch::clock::getTimeMilliseconds();
    uint32_t dt = currTime - prevTime;
    prevTime = currTime;

    Remote* remote = &drivers->remote;

    if (fabs(remote->getChannel(Remote::Channel::LEFT_HORIZONTAL)) > 0.1f) {
        float yawSetpoint = subsystem->getYawTurret()->getAngle() + controllerScalarYaw * -remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
        subsystem->getYawTurret()->setAngle(yawSetpoint, dt);
        subsystem->setPreviousChassisRelativeYawSetpoint(tap::algorithms::ContiguousFloat(yawSetpoint + subsystem->getChassisOffset() * subsystem->BELT_RATIO, 0.0f, M_TWOPI).getValue());
    }

    else {
        subsystem->getYawTurret()->setAngle(subsystem->getTurretWithOffset(), dt);
    }


    if (fabs(remote->getChannel(Remote::Channel::LEFT_VERTICAL)) > 0.1f)  {
        pitch += controllerScalarPitch * remote->getChannel(Remote::Channel::LEFT_VERTICAL);
    }

    else {        
        drivers->terminal << subsystem->getPitchTurret()->getAngle() << "\n";
    }

    pitch = std::clamp(pitch, pitchLowerLimit, pitchUpperLimit);
    subsystem->getPitchTurret()->setAngle(pitch, dt);
}

void TurretCommand::end(bool) {}

bool TurretCommand::isFinished(void) const { return false; }
}  // namespace turret
}  // namespace subsystems
