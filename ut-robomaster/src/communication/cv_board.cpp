#include "cv_board.hpp"

#include "drivers.hpp"

namespace communication
{
#ifdef CV_SPI
CVBoard::CVBoard(src::Drivers* drivers)
    : DJISerial(drivers, SPI_PORT),
      drivers(drivers),
      lastTurretData(),
      offlineTimeout()>
{
    lastTurretData.hasTarget = false;
}
#else  // UART
CVBoard::CVBoard(src::Drivers* drivers)
    : DJISerial(drivers, UART_PORT),
      drivers(drivers),
      lastTurretData(),
      offlineTimeout()
{
    lastTurretData.hasTarget = false;
}
#endif

void CVBoard::initialize()
{
#ifdef CV_SPI
    drivers->spi.init();
#else  // UART
    drivers->uart.init<UART_PORT, BAUD_RATE>();
#endif
}

void CVBoard::messageReceiveCallback(const ReceivedSerialMessage& message)
{
    offlineTimeout.restart(OFFLINE_TIMEOUT_MS);

    switch (message.messageType)
    {
        case CV_MESSAGE_TYPE_TURRET_AIM:
            decodeTurretData(message);
            break;
        // case CV_MESSAGE_TYPE_ECHO:
        // echoData(message);
        case CV_MESSAGE_TYPE_POSITION_REQUEST:
            decodePositionRequest(message);
            break;
        default:
            break;
    }
}

void CVBoard::sendMessage()
{
    // Odometry Data currently has a dummy message
    sendOdometryData();
    // sendColorData();
}

//  void CVBoard::echoData(const ReceivedSerialMessage& message)
// {
//     std::string data_string;
//     memcpy(&data_string, &message.data, sizeof(message.header.dataLength));

//     DJISerial::SerialMessage<sizeof(data_string)> msg;
//     msg.messageType = CV_MESSAGE_TYPE_ECHO;

//     std::string* data = reinterpret_cast<std::string*>(msg.data);

//     data = &data_string;

//     msg.setCRC16();
//     drivers->uart.write(UART_PORT, reinterpret_cast<uint8_t*>(&msg), sizeof(msg));
// }

void CVBoard::sendOdometryData()
{
    DJISerial::SerialMessage<sizeof(OdometryData)> message;
    message.messageType = CV_MESSAGE_TYPE_ODOMETRY_DATA;

    // TODO: Implement sending of data once odometry module is finished

    // Create dummy message
    // struct OdometryData
    // {
    //     float xPos;
    //     float yPos;
    //     float zPos;

    //     float chassisPitch;
    //     float chassisYaw;
    //     float chassisRoll;

    //     float turretPitch;
    //     float turretYaw;
    // } modm_packed;
    OdometryData dummyOdom = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    // Convert data into bytes to send (uart.write() handles the sending of message and num of bytes
    // to transmit across UART PORT) Conversion of databytes handled with memcpy, &message.data is
    // already is data buff array
    memcpy(
        &message.data,
        &dummyOdom,
        sizeof(OdometryData));  // Size of the DataType being copied over

    message.setCRC16();
    drivers->uart.write(UART_PORT, reinterpret_cast<uint8_t*>(&message), sizeof(message));
}

void CVBoard::sendColorData()
{
    DJISerial::SerialMessage<sizeof(ColorData)> message;
    message.messageType = CV_MESSAGE_TYPE_COLOR_DATA;

    ColorData* data = reinterpret_cast<ColorData*>(message.data);
    if (drivers->refSerial.getRefSerialReceivingData())
    {
        bool isBlue = drivers->refSerial.isBlueTeam(drivers->refSerial.getRobotData().robotId);
        data->color = isBlue ? COLOR_BLUE : COLOR_RED;
    }
    else
    {
        data->color = COLOR_UNKNOWN;
    }

    message.setCRC16();
    drivers->uart.write(UART_PORT, reinterpret_cast<uint8_t*>(&message), sizeof(message));
}

bool CVBoard::decodeTurretData(const ReceivedSerialMessage& message)
{
    if (message.header.dataLength == sizeof(lastTurretData))
    {
        memcpy(&lastTurretData, &message.data, sizeof(lastTurretData));
        turretDataIndex += 1;
        return true;
    }
    return false;
}

bool CVBoard::decodePositionRequest(const ReceivedSerialMessage& message)
{
    if (message.header.dataLength == sizeof(lastPositionRequest))
    {
        memcpy(&lastPositionRequest, &message.data, sizeof(lastPositionRequest));
        return true;
    }
    return false;
}

bool CVBoard::isOnline() const { return !offlineTimeout.isExpired(); }
const TurretData& CVBoard::getTurretData() const { return lastTurretData; }
const PositionRequest& CVBoard::getPositionRequest() const { return lastPositionRequest; }
}  // namespace communication
