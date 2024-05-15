#include "communication/uart_test.hpp"

#include "drivers.hpp"

UartTest::UartTest(src::Drivers* drivers) : drivers(drivers) {}
void UartTest::initialize() { drivers->uart.init<Uart::UartPort::Uart6, 115200>(); }
void UartTest::update()
{
    int got = drivers->uart.read(Uart::UartPort::Uart6, buffer, 100);
    if (got > 0)
    {
        received += got;
        updates += 1;
    }
}