#include "communication/uart_test.hpp"

#include "drivers.hpp"

UartTest::UartTest(src::Drivers* drivers) : drivers(drivers) {}
void UartTest::initialize() { drivers->uart.init<Uart::UartPort::Uart6, 115200>(); }
void UartTest::update() { received += drivers->uart.read(Uart::UartPort::Uart6, buffer, 100); }