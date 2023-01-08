#include "tap/communication/serial/terminal_serial.hpp"

using namespace tap::communication::serial;

namespace subsystems
{
namespace test
{
class TestTerminalCallback : public TerminalSerialCallbackInterface
{
public:
    bool terminalSerialCallback(
        char *inputLine,
        modm::IOStream &outputStream,
        bool streamingEnabled);

    void terminalSerialStreamCallback(modm::IOStream &outputStream);

    float targetAngle = 0.0f;
    float currentAngle = 0.0f;
    float pidOutput = 0.0f;
    float deltaTime = 0.0f;
    float arg = 0.0f;
};
}  // namespace test
}  // namespace subsystems