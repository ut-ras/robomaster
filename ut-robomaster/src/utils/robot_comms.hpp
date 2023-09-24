#ifndef ROBOT_COMMS_HPP_
#define ROBOT_COMMS_HPP_

#include "tap/communication/serial/terminal_serial.hpp"
#include "tap/communication/serial/uart_terminal_device.hpp"
#include "tap/drivers.hpp"

namespace comms
{

class RobotComms : public tap::communication::serial::TerminalSerialCallbackInterface
{
    // make this a singleton
public:
    static constexpr char DEFAULT_HEADER[] = "startcomm";

    RobotComms(tap::Drivers *drivers) : RobotComms(drivers, DEFAULT_HEADER) {}
    RobotComms(tap::Drivers *drivers, const char *header);

    bool terminalSerialCallback(
        char *inputLine,
        modm::IOStream &outputStream,
        bool streamingEnabled) override;

    void terminalSerialStreamCallback(modm::IOStream &outputStream) override;

    // Print format like printf, triggered by "startcomm -S" or similar
    void printf(const char *format, ...)
    {
        // concatenate the string to robotStream
        va_list args;
        va_start(args, format);
        robotStreamIndex += vsnprintf(
            robotStream + robotStreamIndex,
            sizeof(robotStream) - robotStreamIndex,
            format,
            args);
        va_end(args);
    }

    // Print parameter by stream, triggered by function call
    RobotComms &operator<<(const char *str);
    RobotComms &operator<<(const int &v);
    RobotComms &operator<<(const float &v);

private:
    const char *const HEADER;

    // robot writes to this stream, and this stream is written to TerminalSerial's stream
    char robotStream[1000];
    uint32_t robotStreamIndex = 0;

    tap::communication::serial::UartTerminalDevice device;
    modm::IOStream stream;
};

}  // namespace comms

#endif