#include "test_terminal_callback.hpp"

#include "modm/math/geometry/angle.hpp"

namespace testing
{
bool TestTerminalCallback::terminalSerialCallback(
    char* inputLine,
    modm::IOStream& o,
    bool streamingEnabled)
{
    if (!streamingEnabled)
    {
        argument = atof(inputLine);
        o << output << modm::endl;
    }
    return true;
}

void TestTerminalCallback::terminalSerialStreamCallback(modm::IOStream&) {}
}  // namespace testing