#include "tap/communication/serial/terminal_serial.hpp"

using namespace tap::communication::serial;

namespace testing
{
class TestTerminalCallback : public TerminalSerialCallbackInterface
{
public:
    bool terminalSerialCallback(
        char *inputLine,
        modm::IOStream &outputStream,
        bool streamingEnabled);

    void terminalSerialStreamCallback(modm::IOStream &outputStream);

    float argument = 0.0f;
    float output = 0.0f;
};
}  // namespace testing