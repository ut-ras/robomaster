#pragma once

#include "tap/communication/serial/ref_serial_data.hpp"
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
        return this->refSerial.getGameData().gameStage ==
               tap::communication::serial::RefSerialData::Rx::GameStage::IN_GAME;
    }
};  // class Drivers

}  // namespace src
