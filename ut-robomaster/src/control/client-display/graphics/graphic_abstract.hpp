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
    graphic_abstract(src::Drivers* drivers, uint8_t id)
        : drivers(drivers),
          refSerialTransmitter(drivers)
    {
        graphicId[0] = id & 0x01;
        graphicId[1] = id & 0x02;
        graphicId[2] = id & 0x04;
    };
    virtual void initialize() = 0;
    virtual ResumableResult<bool> run() = 0;

    // virtual ~graphic_abstract();

protected:
    src::Drivers* drivers;
    RefSerialTransmitter refSerialTransmitter;
    uint8_t graphicId[3];  // 3 byte identifier for this graphic element
    RefSerialData::Tx::Graphic1Message msg;
    RefSerialData::Tx::GraphicCharacterMessage word_msg;

};

}  // namespace graphic