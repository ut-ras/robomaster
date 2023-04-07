#include "drivers.hpp"
#include "beaglebone_communicator.hpp"

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
    DJISerial::SerialMessage<sizeof(OdometryData)> message;
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

bool BeagleBoneCommunicator::setTurret(){
    //take lastTurretData, write to turret reference
    Vector3f position;
    position.x = lastTurretData.xPos;
    position.y = lastTurretData.yPos;
    position.z = lastTurretData.zPos;
    Vector3f velocity;
    velocity.x = lastTurretData.xVel;
    velocity.y = lastTurretData.yVel;
    velocity.z = lastTurretData.zVel;
    Vector3f acceleration;
    acceleration.x = lastTurretData.xAcc;
    acceleration.y = lastTurretData.yAcc;
    acceleration.z = lastTurretData.zAcc;
    turret->inputTargetData(position, velocity, acceleration);
    return false;
}

bool BeagleBoneCommunicator::isBeagleBoneOnline() const {
    return !beagleboneOfflineTimeout.isExpired();
}

const TurretData& BeagleBoneCommunicator::getTurretData() const {
    return lastTurretData;
}
}   // namespace communication
