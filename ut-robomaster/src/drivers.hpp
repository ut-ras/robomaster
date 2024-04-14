#ifndef DRIVERS_HPP_
#define DRIVERS_HPP_

#include "tap/drivers.hpp"

#include "communication/cv_board.hpp"
#include "drivers/board.hpp"
#include "drivers/encoder.hpp"
#include "utils/robot_comms.hpp"

namespace src
{
class Drivers : public tap::Drivers
{
    friend class DriversSingleton;

#ifdef ENV_UNIT_TESTS
public:
#endif
    Drivers() : tap::Drivers(), cvBoard(this), terminal(this), encoder() {}

public:
    communication::CVBoard cvBoard;
    comms::RobotComms terminal;
    encoder::Encoder<Board::I2cMaster> encoder;
    bool isKillSwitched() { return !remote.isConnected(); }
};  // class Drivers

}  // namespace src

#endif  // DRIVERS_HPP_
