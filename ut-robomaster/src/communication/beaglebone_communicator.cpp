#include "drivers.hpp"
#include "beaglebone_communicator.hpp"
#include "tap/communication/serial/uart.hpp"


namespace communication {

BeagleBoneCommunicator::BeagleBoneCommunicator(src::Drivers* drivers)
    : DJISerial(drivers, BEAGLEBONE_UART_PORT),
      drivers(drivers),
      lastTurretData(),
      beagleboneOfflineTimeout()
{
    lastTurretData.hasTarget = false;
}

void BeagleBoneCommunicator::initialize() {
    drivers->uart.init<BEAGLEBONE_UART_PORT, BEAGLEBONE_BAUD_RATE>();
}

void BeagleBoneCommunicator::messageReceiveCallback(const ReceivedSerialMessage& message) {
    beagleboneOfflineTimeout.restart(BEAGLEBONE_OFFLINE_TIMEOUT_MS);
    switch (message.messageType)
    {
        case CV_MESSAGE_TYPE_TURRET_AIM:
        {
            decodeTurretData(message);
            return;
        }
        default:
            return;
    }
}

void BeagleBoneCommunicator::sendMessage() {
    sendOdometryData();
}

void BeagleBoneCommunicator::sendOdometryData() {
    //DJISerial::SerialMessage<sizeof(OdometryData)> message;


    
    //message.data = data;
    message.messageType = CV_MESSAGE_TYPE_ODOMETRY_DATA;

    // TODO: Implement sending of data once odometry module is finished

    message.setCRC16();
    drivers->uart.write(BEAGLEBONE_UART_PORT, reinterpret_cast<uint8_t*>(&message), sizeof(message));



}

bool BeagleBoneCommunicator::decodeTurretData(const ReceivedSerialMessage& message) {
    if (message.header.dataLength == sizeof(lastTurretData)) {
        memcpy(&lastTurretData, &message.data, sizeof(lastTurretData));
        return true;
    }
    
    return false;
}

bool BeagleBoneCommunicator::isBeagleBoneOnline() const {
    return !beagleboneOfflineTimeout.isExpired();
}

const TurretData& BeagleBoneCommunicator::getTurretData() const {
    return lastTurretData;
}
}   // namespace communication
