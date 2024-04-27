#ifndef DRIVERS_HPP_
#define DRIVERS_HPP_

#include "tap/drivers.hpp"

#include "communication/cv_board.hpp"
#include "communication/uart_test.hpp"
#include "utils/robot_comms.hpp"

namespace src
{
class Drivers : public tap::Drivers
{
    friend class DriversSingleton;

#ifdef ENV_UNIT_TESTS
public:
#endif
    Drivers() : tap::Drivers(), cvBoard(this), terminal(this), uartTest(this) {}

public:
    communication::CVBoard cvBoard;
    comms::RobotComms terminal;
    UartTest uartTest;

    bool isKillSwitched() { return !remote.isConnected(); }
};  // class Drivers

}  // namespace src

#endif  // DRIVERS_HPP_
