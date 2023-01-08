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
    int a = strcmp(inputLine, "hi");

    outputStream << "oaky" << modm::endl;

    if (a == 0)
    {
        outputStream << "same" << modm::endl;
    }
    else
    {
        outputStream << "diffy" << modm::endl;
    }

    return true;
}

void TestTerminalCallback::terminalSerialStreamCallback(modm::IOStream& o)
{
    o << static_cast<int>((currentAngle / M_TWOPI * 360.0f)) << modm::endl;
}
}  // namespace test
}  // namespace subsystems
