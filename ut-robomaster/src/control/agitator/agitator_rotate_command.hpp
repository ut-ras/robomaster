#ifndef AGITATOR_ROTATE_COMMAND_HPP_
#define AGITATOR_ROTATE_COMMAND_HPP_

#include "tap/control/command.hpp"

namespace control
{
namespace agitator
{
class AgitatorSubsystem;
class AgitatorRotateCommand : public tap::control::Command
{
public:
    AgitatorRotateCommand(AgitatorSubsystem *sub)
        : agitator(sub)
    {
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "agitator rotate command"; }

private:
    AgitatorSubsystem *agitator;

};  // class AgitatorRotateCommand
}  // namespace agitator
}  // namespace control

#endif  // AGITATOR_ROTATE_COMMAND_HPP_
