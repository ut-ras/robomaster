#pragma once

#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands 
{
using subsystems::turret::TurretSubsystem;
using tap::communication::serial::Remote; 

class CommandAimStrategy : public tap::control::Command 
{
public:
    CommandAimStrategy(src::Drivers* drivers, TurretSubsystem* turrent) 
        : drivers(drivers),
          turret(turret) 
        {}

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char *getName() const override { return "change aim strategy command"; }

private:
    src::Drivers* drivers;
    TurretSubsystem* turret;
};  // class CommandAimStrategy
}  // namespace commands

