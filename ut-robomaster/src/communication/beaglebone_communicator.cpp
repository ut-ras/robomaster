#include "beaglebone_communicator.hpp"

#include "drivers.hpp"

namespace communication
{
BeagleBoneCommunicator::BeagleBoneCommunicator(src::Drivers* drivers)
    : DJISerial(drivers, UART_PORT),
      drivers(drivers),
      lastTurretData(),
      offlineTimeout()
{
    lastTurretData.hasTarget = false;
}

void BeagleBoneCommunicator::initialize()
{
    drivers->uart.init<UART_PORT, BAUD_RATE>();
    drivers->terminal << "init\n";
}

void BeagleBoneCommunicator::messageReceiveCallback(const ReceivedSerialMessage& message)
{
    drivers->terminal << "msg ";

    offlineTimeout.restart(OFFLINE_TIMEOUT_MS);

    switch (message.messageType)
    {
        case CV_MESSAGE_TYPE_TURRET_AIM:
        {
            drivers->terminal << "trt: ";
            decodeTurretData(message);
            drivers->terminal << lastTurretData.xPos;
            drivers->terminal << ", ";
            drivers->terminal << lastTurretData.yPos;
            drivers->terminal << ", ";
            drivers->terminal << lastTurretData.zPos;
            drivers->terminal << "\n";
            return;
        }
        default:
            drivers->terminal << "other\n";
            return;
    }
}

void BeagleBoneCommunicator::sendMessage() { sendOdometryData(); }

void BeagleBoneCommunicator::sendOdometryData()
{
    DJISerial::SerialMessage<sizeof(OdometryData)> message;
    message.messageType = CV_MESSAGE_TYPE_ODOMETRY_DATA;

    // TODO: Implement sending of data once odometry module is finished

    message.setCRC16();
    drivers->uart.write(UART_PORT, reinterpret_cast<uint8_t*>(&message), sizeof(message));
}

bool BeagleBoneCommunicator::decodeTurretData(const ReceivedSerialMessage& message)
{
    if (message.header.dataLength == sizeof(lastTurretData))
    {
        memcpy(&lastTurretData, &message.data, sizeof(lastTurretData));
        // if(lastTurretData.hasTarget){
        //     //setTurret();
        // }
        // else{
        //     //turret->setAimStrategy(subsystems::turret::AimStrategy::Manual);
        // }
        return true;
    }
    return false;
}
/*
void BeagleBoneCommunicator::setTurret(){
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
    //turret->inputTargetData(position, velocity, acceleration);
}*/

bool BeagleBoneCommunicator::isOnline() const { return !offlineTimeout.isExpired(); }

void BeagleBoneCommunicator::test()
{
    uint8_t b;
    bool got = drivers->uart.read(UART_PORT, &b);

    if (got)
    {
        drivers->terminal << "got: ";
        drivers->terminal << b;
        drivers->terminal << "\n";
    }
}

const TurretData& BeagleBoneCommunicator::getTurretData() const { return lastTurretData; }
}  // namespace communication
