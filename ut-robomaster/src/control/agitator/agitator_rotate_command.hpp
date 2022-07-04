#ifndef AGITATOR_ROTATE_COMMAND_HPP_
#define AGITATOR_ROTATE_COMMAND_HPP_

#include "tap/control/command.hpp"
#include "tap/architecture/periodic_timer.hpp"
#include "agitator_subsystem.hpp"

namespace control
{
namespace agitator
{
class AgitatorRotateCommand : public tap::control::Command
{
public:
    AgitatorRotateCommand(AgitatorSubsystem *sub)
        : agitator(sub)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "agitator rotate command"; }

private:
    AgitatorSubsystem *agitator;
    int16_t jamTorque = 200;
    float rotateRPM = 2000.0f;
    bool isUnjamming;
    float prevTime;
};  // class AgitatorRotateCommand
}  // namespace agitator
}  // namespace control

#endif  // AGITATOR_ROTATE_COMMAND_HPP_
