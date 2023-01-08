#include "test_command.hpp"

namespace subsystems
{
namespace test
{
void TestCommand::initialize() {}

void TestCommand::execute()
{
    Remote* remote = &drivers->remote;
    float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
    float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);
    subsystem->input(h, v);
}

void TestCommand::end(bool interrupted) { subsystem->input(0.0f, 0.0f); }
bool TestCommand::isFinished(void) const { return false; }
}  // namespace test
}  // namespace subsystems
