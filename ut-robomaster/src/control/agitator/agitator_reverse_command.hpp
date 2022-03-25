#ifndef AGITATOR_REVERSE_COMMAND_HPP_
#define AGITATOR_REVERSE_COMMAND_HPP_

#include "tap/control/command.hpp"

namespace control
{
namespace agitator
{
class AgitatorSubsystem;
class AgitatorReverseCommand : public tap::control::Command
{
public:
    AgitatorReverseCommand(AgitatorSubsystem *sub)
        : agitator(sub)
    {
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "agitator reverse command"; }

private:
    AgitatorSubsystem *agitator;

};  // class AgitatorReverseCommand
}  // namespace agitator
}  // namespace control

#endif  // AGITATOR_REVERSE_COMMAND_HPP_
