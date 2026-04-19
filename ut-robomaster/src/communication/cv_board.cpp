
#include "cv_board.hpp"

#include "tap/communication/serial/ref_serial_data.hpp"

#include "subsystems/odometry/odometry_subsystem.hpp"

#include "drivers.hpp"
#include "rtt.hpp"
namespace src
{

namespace communication
{

CVBoard::CVBoard(src::Drivers* drivers, OdometrySubsystem* odometry)
    : DJISerial(drivers, UART_PORT),
      drivers(drivers),
      lastTurretData(),
      offlineTimeout(),
      odometry(odometry)

{
    lastTurretData.hasTarget = false;
}

void CVBoard::initialize()
{
#ifdef CV_SPI
    // Default to Spi2
    spi_interface.init<serial::Spi::SpiPort::Spi2, 1'320'000>();  // 1 MHz for now (just guessing)
                                                                  // // Jiyan's note: I changed
                                                                  // this to set is at the enum
                                                                  // value but not sure if it'll
                                                                  // work. Needs clarification
#else                                                             // UART
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

void CVBoard::printSPIMessage()
{
    // First, check if you can read
    if (spi_interface.isReceiveRegisterNotEmpty(SPI_PORT))  // A message was received
    {
        // Read the message
        uint8_t message;
        spi_interface.read(SPI_PORT, &message);
        drivers->rtt << message
                     << '\n';  // To be able to do this, I had to create an rtt stream object in
                               // the cv_board.hpp file, for more details, I believe a guide on
                               // how to use rtt is available on modm's website -- Jiyan
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

    OdometryData dummyOdom = {1.0, 2.0, 3.0};
    OdometryMessage dummyOdomMessage;
    tap::communication::serial::RefSerialData::Rx::RobotData robotData =
        drivers->refSerial.getRobotData();  // obtain robot data

    // Prepare the main message body

    dummyOdom.health = robotData.currentHp;

    dummyOdomMessage.odometryData = dummyOdom;

#ifdef CV_SPI

    uint8_t* data_stream =
        reinterpret_cast<uint8_t*>(&dummyOdomMessage);  // Prepare the data to send it as a stream
    for (uint8_t i = 0; i < sizeof(dummyOdomMessage); i++)
    {
        if (spi_interface.isTransmitRegisterEmpty(SPI_PORT) == false)
        {
            i--;
            continue;
            // Wait until the register becomes available if can't transmit
        }
        spi_interface.write(SPI_PORT, data_stream[i]);
    }
    /*
    // Check if can send message
    if (spi_interface.isTransmitRegisterEmpty(SPI_PORT) == true)
    {
        /* uint8_t* data_stream =
            reinterpret_cast<uint8_t*>(&dummyOdom);  // Prepare the data to send it as a stream
        for (uint8_t i = 0; i < sizeof(data_stream); i++)
        {
            if (i == 0)
            {
                // Send the first 2 bytes that indicate the beginning of the communication
            }
            spi_interface.write(SPI_PORT, data_stream[i]);
        }
    static uint8_t var = 78;
    spi_interface.write(SPI_PORT, var);
    var++;
}
*/

#else
    DJISerial::SerialMessage<sizeof(OdometryData)> message;
    message.messageType = CV_MESSAGE_TYPE_ODOMETRY_DATA;

    // Convert data into bytes to send (uart.write() handles the sending of message and num of bytes
    // to transmit across UART PORT) Conversion of databytes handled with memcpy, &message.data is
    // already is data buff array
    memcpy(
        &message.data,
        &dummyOdom,
        sizeof(OdometryData));  // Size of the DataType being copied over

    message.setCRC16();
    drivers->uart.write(UART_PORT, reinterpret_cast<uint8_t*>(&message), sizeof(message));
#endif
}

void CVBoard::sendColorData()
{
#ifdef CV_SPI
    // TODO: To fix
    OdometryData dummyOdom = {1.0, 2.0, 3.0};
    // Check if can send message
    if (spi_interface.isTransmitRegisterEmpty(SPI_PORT) == false && GpioB12::read() == true)
    {
        uint16_t var = 35;
        spi_interface.write(SPI_PORT, var);
    }
#else
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
#endif
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
}  // namespace src