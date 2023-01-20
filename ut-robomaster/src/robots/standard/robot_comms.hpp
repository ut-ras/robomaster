#include "tap/communication/serial/terminal_serial.hpp"

namespace comms
{

class RobotComms : public tap::communication::serial::TerminalSerialCallbackInterface
{
    // make this a singleton
public:
    static constexpr char HEADER[] = "startcomm";

    RobotComms(tap::Drivers* drivers) : drivers_(drivers) {}
    mockable void init();

    bool terminalSerialCallback(
        char* inputLine,
        modm::IOStream& outputStream,
        bool streamingEnabled) override;

    void terminalSerialStreamCallback(modm::IOStream& outputStream) override;

    // robot writes to this stream, and this stream is written to TerminalSerial's stream
    char robotStream[1000];
    uint32_t robotStreamIndex = 0;

private:
    tap::Drivers* drivers_;
};

class RobotCommsSingleton
{
public:
    static RobotComms* instance_;

    static void init(tap::Drivers* drivers)
    {
        instance_ = new RobotComms(drivers);
        instance_->init();
    }

    static RobotComms& getInstance() { return *instance_; }

    static void print(const char* format, ...)
    {
        // concatenate the string to robotStream
        va_list args;
        va_start(args, format);
        instance_->robotStreamIndex += vsnprintf(
            instance_->robotStream + instance_->robotStreamIndex,
            sizeof(instance_->robotStream) - instance_->robotStreamIndex,
            format,
            args);
        va_end(args);
    }
};

}  // namespace comms
