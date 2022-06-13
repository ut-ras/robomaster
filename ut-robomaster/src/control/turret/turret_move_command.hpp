#ifndef TURRET_MOVE_COMMAND_HPP_
#define TURRET_MOVE_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "turret_subsystem.hpp"

class Drivers;

namespace control
{
namespace turret
{
class TurretMoveCommand : public tap::control::Command
{
public:
    TurretMoveCommand(tap::Drivers *drivers, TurretSubsystem *sub);

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "turret move command"; }

private:
    tap::Drivers *drivers;
    TurretSubsystem *turret;
};  // class TurretDriveCommand
}  // namespace turret
}  // namespace control

#endif  // TURRET_DRIVE_COMMAND_HPP_
