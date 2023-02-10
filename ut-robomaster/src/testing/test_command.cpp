#include "test_command.hpp"

namespace testing
{
void TestCommand::initialize() {}

void TestCommand::execute()
{
    Remote* remote = &drivers->remote;
    float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
    float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);
    subsystem->input(h, v);
}

void TestCommand::end(bool) { subsystem->input(0.0f, 0.0f); }
bool TestCommand::isFinished(void) const { return false; }
}  // namespace testing
