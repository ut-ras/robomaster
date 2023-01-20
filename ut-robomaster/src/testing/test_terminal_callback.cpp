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
        arg = atof(inputLine);
        o << lastMeasurement << modm::endl;
    }
    return true;
}

void TestTerminalCallback::terminalSerialStreamCallback(modm::IOStream& o)
{
    o << arg << modm::endl;
}
}  // namespace testing