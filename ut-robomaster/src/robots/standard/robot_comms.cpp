#include "robot_comms.hpp"

namespace comms
{
RobotComms::RobotComms(tap::Drivers *drivers, const char *header)
    : HEADER(header), device(drivers), stream(device)
{
    drivers->terminalSerial.addHeader(HEADER, this);
}

void RobotComms::terminalSerialStreamCallback(modm::IOStream& outputStream)
{
    outputStream << robotStream << modm::endl;
    outputStream.flush();
    memset(robotStream, 0, sizeof(robotStream));
    robotStreamIndex = 0;
}

bool RobotComms::terminalSerialCallback(
    char* inputLine,
    modm::IOStream& outputStream,
    bool streamingEnabled)
{
    // remove everything from robotStream and write to outputStream
    outputStream << robotStream << modm::endl;
    outputStream.flush();
    memset(robotStream, 0, sizeof(robotStream));
    robotStreamIndex = 0;
    streamingEnabled = true;
    return true;
}

RobotComms &RobotComms::operator<<(const char *str)
{
    stream << str;
    return *this;
}
RobotComms &RobotComms::operator<<(const int &v)
{
    stream << v;
    return *this;
}
RobotComms &RobotComms::operator<<(const float &v)
{
    stream << v;
    return *this;
}

}  // namespace comms