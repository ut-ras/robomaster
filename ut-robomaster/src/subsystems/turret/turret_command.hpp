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

/**
 * This code is part of aruw's repository
 *
 * Example code for a default command for the subsystem-example subsystem.
 */

#ifndef TURRET_COMMAND_HPP_
#define TURRET_COMMAND_HPP_

#include "tap/control/command.hpp"
#include "drivers.hpp"
#include "tap/communication/serial/remote.hpp"

class TurretSubsystem;

class TurretCommand : public tap::control::Command
{
public:
    static constexpr int16_t DEFAULT_WHEEL_RPM = 2000;

    TurretCommand(tap::Drivers* drivers, TurretSubsystem* subsystem);

    /**
     * The initial subroutine of a command.  Called once when the command is
     * initially scheduled.
     */
    void initialize() override;

    /**
     * The main body of a command.  Called repeatedly while the command is
     * scheduled.
     */
    void execute() override;

    /**
     * The action to take when the command ends.  Called when either the command
     * finishes normally, or when it interrupted/canceled.
     *
     * @param interrupted whether the command was interrupted/canceled
     */
    void end(bool interrupted) override;

    /**
     * Whether the command has finished.  Once a command finishes, the scheduler
     * will call its end() method and un-schedule it.
     *
     * @return whether the command has finished.
     */
    bool isFinished() const override;

    const char* getName() const override { return "example"; }

private:
    TurretSubsystem* subsystem;

    tap::Drivers* drivers;
};

#endif  // EXAMPLE_COMMAND_HPP_
