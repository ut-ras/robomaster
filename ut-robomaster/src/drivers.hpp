#pragma once

#include "tap/drivers.hpp"

#include "communication/cv_board.hpp"
#include "communication/rtt.hpp"

namespace src
{
class Drivers : public tap::Drivers
{
public:
    Drivers() : tap::Drivers(), cvBoard(this), rtt() {}
    communication::CVBoard cvBoard;
    communication::RttStream rtt;

    bool isKillSwitched() { return !remote.isConnected(); }
    bool isGameActive()
    {
        // gamestage 4 is IN_GAME
        return this->refSerial.getGameData().gameStage == 4
    }
};  // class Drivers

}  // namespace src
