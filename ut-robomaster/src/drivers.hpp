#pragma once

#include "tap/drivers.hpp"

#include "communication/cv_board.hpp"

namespace src
{
class Drivers : public tap::Drivers
{
public:
    Drivers() : tap::Drivers(), cvBoard(this) {}
    communication::CVBoard cvBoard;

    bool isKillSwitched() { return !remote.isConnected(); }
};  // class Drivers

}  // namespace src
