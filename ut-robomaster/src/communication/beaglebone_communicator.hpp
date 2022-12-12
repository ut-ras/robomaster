#pragma once

#include <tap/util_macros.hpp>
#include "tap/architecture/timeout.hpp"
#include "tap/communication/serial/dji_serial.hpp"
#include "tap/communication/serial/uart.hpp"

namespace src {
class Drivers;
}

class BeagleBoneCommunicator : public tap::communication::serial::DJISerial {
public:
    BeagleBoneCommunicator(src::Drivers* drivers);
    DISALLOW_COPY_AND_ASSIGN(BeagleBoneCommunicator);
    ~BeagleBoneCommunicator() = default;

    /**
     * Initializes the UART line and callback interface, UART defaults to Uart6
     * 
     * Note: Uart6 is utilized by the terminal serial; to ensure smooth operation, 
     * comment out the initialize and update calls in main
     */
    void initialize();

    /**
     * Overload of initialize() to allow for setting of UART port
     * 
     * @param uartPort UART port to set to
     */
    void initialize(tap::communication::serial::Uart::UartPort uartPort);

    /**
     * Processes messages received from the BeagleBone
     * 
     * @param completeMessage The message received by the callback handler; see dji_serial.hpp
     * for more information about the structure of the message
     */
    void messageReceiveCallback(const ReceivedSerialMessage& completeMessage) override;

    /**
     * Sends messages to the BeagleBone
     */
    void sendMessage();

    /**
     * @return true if a message has been received within the last 
     * BEAGLEBONE_OFFLINE_TIMEOUT_MS milliseconds, false if otherwise
     */
    bool isBeagleBoneOnline() const;


private:
    static constexpr tap::communication::serial::Uart::UartPort BEAGLEBONE_UART_PORT = tap::communication::serial::Uart::UartPort::Uart6;
    static constexpr uint16_t BEAGLEBONE_OFFLINE_TIMEOUT_MS = 2000;
};