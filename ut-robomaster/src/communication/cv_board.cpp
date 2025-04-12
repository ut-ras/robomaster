#include "cv_board.hpp"

#include "drivers.hpp"

namespace communication
{
CVBoard::CVBoard(src::Drivers* drivers)
    : DJISerial(drivers, UART_PORT),
      drivers(drivers),
      lastTurretData(),
      offlineTimeout()
{
    lastTurretData.hasTarget = false;
}

void CVBoard::initialize() { drivers->uart.init<UART_PORT, BAUD_RATE>(); }

void CVBoard::messageReceiveCallback(const ReceivedSerialMessage& message)
{
    offlineTimeout.restart(OFFLINE_TIMEOUT_MS);

    switch (message.messageType)
    {
        case CV_MESSAGE_TYPE_TURRET_AIM:
            decodeTurretData(message);
            break;
        case CV_MESSAGE_TYPE_ECHO:
            echoData(message);
        default:
            break;
    }
}

void CVBoard::sendMessage()
{
    sendOdometryData();
    sendColorData();
}

void CVBoard::echoData(const ReceivedSerialMessage& message)
{
    std::string data_string;
    memcpy(&data_string, &message.data, sizeof(message.header.dataLength));

    DJISerial::SerialMessage<sizeof(data_string)> msg;
    msg.messageType = CV_MESSAGE_TYPE_ECHO;

    std::string* data = reinterpret_cast<std::string*>(msg.data);

    data = &data_string;

    msg.setCRC16();
    drivers->uart.write(UART_PORT, reinterpret_cast<uint8_t*>(&msg), sizeof(msg));
}

void CVBoard::sendOdometryData()
{
    DJISerial::SerialMessage<sizeof(OdometryData)> message;
    message.messageType = CV_MESSAGE_TYPE_ODOMETRY_DATA;

    // TODO: Implement sending of data once odometry module is finished

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

bool CVBoard::isOnline() const { return !offlineTimeout.isExpired(); }
const TurretData& CVBoard::getTurretData() const { return lastTurretData; }
}  // namespace communication
