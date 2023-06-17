#pragma once

#include "tap/control/command.hpp"

#include "subsystems/shooter/shooter_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::shooter::ShooterSubsystem;

class CommandShooterDefault : public tap::control::Command
{
public:
    CommandShooterDefault(src::Drivers *drivers, ShooterSubsystem *shooter)
        : drivers(drivers),
          shooter(shooter)
    {
        addSubsystemRequirement(shooter);
    }

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char *getName() const override { return "shooter default command"; }

private:
    src::Drivers *drivers;
    ShooterSubsystem *shooter;

};  // class CommandFireContinuous
}  // namespace commands