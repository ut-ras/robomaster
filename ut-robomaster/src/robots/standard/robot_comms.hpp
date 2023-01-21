#include "tap/communication/serial/terminal_serial.hpp"

namespace comms
{

class RobotComms : public tap::communication::serial::TerminalSerialCallbackInterface
{
    // make this a singleton
public:
    static constexpr char HEADER[] = "startcomm";

    RobotComms() {}
    mockable void init(tap::Drivers* drivers);

    bool terminalSerialCallback(
        char* inputLine,
        modm::IOStream& outputStream,
        bool streamingEnabled) override;

    void terminalSerialStreamCallback(modm::IOStream& outputStream) override;

    void print(const char* format, ...)
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

private:
    // robot writes to this stream, and this stream is written to TerminalSerial's stream
    char robotStream[1000];
    uint32_t robotStreamIndex = 0;

};

class RobotCommsSingleton
{
public:
    static void init(tap::Drivers* drivers)
    {
        instance_.init(drivers);
    }

    static RobotComms& getInstance() { return instance_; }

    static void print(const char* format, ...)
    {
        // concatenate the string to robotStream
        va_list args;
        va_start(args, format);
        instance_.print(format, args);
        va_end(args);
    }

private:
    static RobotComms instance_;
};

}  // namespace comms
