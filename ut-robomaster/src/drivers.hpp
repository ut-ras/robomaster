#pragma once

#include "tap/drivers.hpp"

#include "communication/cv_board.hpp"
#include "utils/robot_comms.hpp"

namespace src
{
class Drivers : public tap::Drivers
{
public:
    Drivers() : tap::Drivers(), cvBoard(this), terminal(this) {}
    communication::CVBoard cvBoard;
    comms::RobotComms terminal;
    bool isKillSwitched() { return !remote.isConnected(); }
};  // class Drivers

}  // namespace src
