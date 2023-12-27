#include "command_agitator_burst_alternating.hpp"

namespace commands
{
void CommandAgitatorBurstAlternating::initialize()
{
    auto cmd = isAlternate ? burstCmd2 : burstCmd1;
    comprisedCommandScheduler.addCommand(cmd);
    isAlternate = !isAlternate;
}

void CommandAgitatorBurstAlternating::execute() { comprisedCommandScheduler.run(); }

void CommandAgitatorBurstAlternating::end(bool interrupted)
{
    comprisedCommandScheduler.removeCommand(burstCmd1, interrupted);
    comprisedCommandScheduler.removeCommand(burstCmd2, interrupted);
}

bool CommandAgitatorBurstAlternating::isFinished() const
{
    return !comprisedCommandScheduler.isCommandScheduled(burstCmd1) &&
           !comprisedCommandScheduler.isCommandScheduled(burstCmd2);
}
}  // namespace commands