#include "terminal_callback.hpp"

#include "modm/math/geometry/angle.hpp"

namespace subsystems
{
namespace test
{
bool TestTerminalCallback::terminalSerialCallback(
    char* inputLine,
    modm::IOStream& outputStream,
    bool streamingEnabled)
{
    arg = atof(inputLine);
    return true;
}

void TestTerminalCallback::terminalSerialStreamCallback(modm::IOStream& o)
{
    // o << static_cast<int>((currentAngle / M_TWOPI * 360.0f)) << modm::endl;
    o << arg << ", " << currentAngle << modm::endl;
}
}  // namespace test
}  // namespace subsystems
