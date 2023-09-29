#include "command_rotate_agitator.hpp"

namespace commands
{
using tap::communication::serial::Remote;

void CommandRotateAgitator::initialize() {}

void CommandRotateAgitator::execute()
{
    float factor = drivers->remote.getChannel(Remote::Channel::RIGHT_VERTICAL);
    factor *= abs(factor);  // quadratic ramping

    agitator->setSpeed(factor * MAX_SPEED);
}

void CommandRotateAgitator::end(bool) {}

bool CommandRotateAgitator::isFinished() const { return false; }
}  // namespace commands