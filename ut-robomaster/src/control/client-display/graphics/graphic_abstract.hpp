#pragma once
#include "tap/communication/serial/ref_serial_data.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/control/command.hpp"

#include "modm/processing/protothread.hpp"
#include "modm/processing/resumable.hpp"

#include "drivers.hpp"

using namespace tap::communication::serial;
using modm::NestedResumable;
using modm::ResumableResult;

namespace graphic
{

class graphic_abstract : protected NestedResumable<3>
{
public:
    graphic_abstract(src::Drivers* drivers) : drivers(drivers), refSerialTransmitter(drivers) {};
    virtual ResumableResult<void> initialize() = 0;
    virtual ResumableResult<bool> run() = 0;

    // virtual ~graphic_abstract();

protected:
    //bottom left is (0, 0) top right is (1920, 1080)
    src::Drivers* drivers;
    RefSerialTransmitter refSerialTransmitter;
    uint8_t graphicId[3];  // 3 byte identifier for this graphic element
    RefSerialData::Tx::Graphic1Message msg;
    RefSerialData::Tx::Graphic5Message multiMsg;
    RefSerialData::Tx::GraphicCharacterMessage word_msg;
};

}  // namespace graphic