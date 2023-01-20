#include "robot_comms.hpp"

#include "tap/drivers.hpp"

namespace comms
{

RobotComms* RobotCommsSingleton::instance_ = nullptr;

void RobotComms::terminalSerialStreamCallback(modm::IOStream& outputStream)
{
    outputStream << robotStream << modm::endl;
    outputStream.flush();
    outputStream << "hello" << modm::endl;
    memset(robotStream, 0, sizeof(robotStream));
    robotStreamIndex = 0;
}

void RobotComms::init() { drivers_->terminalSerial.addHeader(HEADER, this); }

bool RobotComms::terminalSerialCallback(
    char* inputLine,
    modm::IOStream& outputStream,
    bool streamingEnabled)
{
    // remove everything from robotStream and write to outputStream
    outputStream << robotStream << modm::endl;
    outputStream.flush();
    outputStream << "hello" << modm::endl;
    memset(robotStream, 0, sizeof(robotStream));
    robotStreamIndex = 0;
    streamingEnabled = true;
    return true;
}

}  // namespace comms