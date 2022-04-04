#ifndef AGITATOR_REVERSE_COMMAND_HPP_
#define AGITATOR_REVERSE_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"

namespace control
{
namespace agitator
{
class AgitatorReverseCommand : public tap::control::Command
{
public:
    AgitatorReverseCommand(AgitatorSubsystem *sub)
        : agitator(sub)
    {
        addSubsystemRequirement(sub);
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
