#pragma once

#include "tap/architecture/timeout.hpp"
#include "tap/communication/serial/dji_serial.hpp"
#include "tap/communication/serial/uart.hpp"
#include "tap/util_macros.hpp"

#include "serial/spi.hpp"

#include "cv_message.hpp"

#define CV_SPI  // Communicate to CV board via SPI (instead of UART)

namespace src
{

class Drivers;
}

namespace communication
{

using src::communication::serial::Spi;
using tap::communication::serial::Uart;

class CVBoard : public tap::communication::serial::DJISerial
{
public:
    CVBoard(src::Drivers* drivers);
    DISALLOW_COPY_AND_ASSIGN(CVBoard);
    virtual ~CVBoard() = default;

#ifdef CV_SPI
    static Spi spi;
#endif

    /**
     * Initializes the UART line and callback interface, UART defaults to Uart1
     * Also inits the SPI line, SPI defaults to SPI2 (custom pin header)
     *
     * Note: Uart1 is utilized by the terminal serial; to ensure smooth operation,
     * comment out the initialize and update calls in main
     */
    void initialize();

    /**
     * Processes messages received from the CV board
     *
     * @param completeMessage The message received by the callback handler; see dji_serial.hpp
     * for more information about the structure of the message
     */
    void messageReceiveCallback(const ReceivedSerialMessage& completeMessage) override;

    /**
     * Sends messages to the CV board
     */
    void sendMessage();

    /**
     * Echo the message sent by the CV board to the CV board
     */
    void echoData(const ReceivedSerialMessage& message);

    /**
     * Sends odometry data to the CV board
     */
    void sendOdometryData();

    /**
     * Sends opposing team's color to the BeagleBone
     */
    void sendColorData();

    /**
     * Decodes the turret aiming data received from the CV board and stores it in lastTurretData
     *
     * @param message The message received from the CV board
     * @return true if message was valid and succesfully decoded, false otherwise
     */
    bool decodeTurretData(const ReceivedSerialMessage& message);

    /**
     * Decode a request to move to a position.
     * @param message message from CV board
     * @return true if valid, false otherwise
     */
    bool decodePositionRequest(const ReceivedSerialMessage& message);

    /**
     * Fetch the position request from the CV board
     */
    const PositionRequest& getPositionRequest() const;

    /**
     * @return true if a message has been received within the last OFFLINE_TIMEOUT_MS milliseconds,
     * false if otherwise
     */
    bool isOnline() const;

    /**
     * @return a reference to lastTurretData (the last turret aiming data received from the
     * CV board)
     */
    const TurretData& getTurretData() const;

    /**
     * Identifier for the last turret data packet received, so we can check for new data
     */
    uint32_t turretDataIndex = 0;

private:
    src::Drivers* drivers;

    /** Last turret aiming data received from the CV board */
    TurretData lastTurretData;

    PositionRequest lastPositionRequest = {
        .x_requested = -999.9f,
        .y_requested = -999.9f,
        .time_sent = 0xdeadbeef};

#ifndef CV_SPI
    /** UART port used to communicate with the CV board */
    static constexpr Uart::UartPort UART_PORT = Uart::Uart1;
    static constexpr uint32_t BAUD_RATE = 115200;
#endif

    /** Time to wait between messages before considering the CV board offline */
    static constexpr uint16_t OFFLINE_TIMEOUT_MS = 2000;

    /** Timer to track elapsed time since last message received */
    tap::arch::MilliTimeout offlineTimeout;
};
}  // namespace communication