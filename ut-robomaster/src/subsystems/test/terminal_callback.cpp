#include "terminal_callback.hpp"

#include "modm/math/geometry/angle.hpp"

namespace subsystems
{
namespace test
{
bool TestTerminalCallback::terminalSerialCallback(
    char* inputLine,
    modm::IOStream& o,
    bool streamingEnabled)
{
    if (!streamingEnabled)
    {
        arg = atof(inputLine);
        o << currentAngle << ", " << pidOutput << modm::endl;
    }
    return true;
}

void TestTerminalCallback::terminalSerialStreamCallback(modm::IOStream& o)
{
    o << arg << ", " << currentAngle << modm::endl;
}
}  // namespace test
}  // namespace subsystems
