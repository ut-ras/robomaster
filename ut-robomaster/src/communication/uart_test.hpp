#pragma once

#include "tap/architecture/timeout.hpp"
#include "tap/communication/serial/dji_serial.hpp"
#include "tap/communication/serial/uart.hpp"
#include "tap/util_macros.hpp"

#include "cv_message.hpp"

namespace src
{
class Drivers;
}

using tap::communication::serial::Uart;

class UartTest
{
public:
    UartTest(src::Drivers* drivers);
    DISALLOW_COPY_AND_ASSIGN(UartTest);
    virtual ~UartTest() = default;

    void initialize();
    void update();

    uint8_t buffer[100];
    uint32_t received = 0;
    uint32_t updates = 0;

private:
    src::Drivers* drivers;
};