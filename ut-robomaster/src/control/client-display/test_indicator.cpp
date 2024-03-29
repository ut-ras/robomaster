// #### TODO: MOVE THIS TO AN HPP FILE
#pragma once

#include "tap/communication/referee/state_hud_indicator.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "tap/util_macros.hpp"
/// #########

using namespace tap::communication::serial;
class TestIndicator
{
    TestIndicator::TestIndicator(RefSerialTransmitter &refSerialTransmitter)
        : refSerialTransmitter(refSerialTransmitter)
    {
        RefSerialTransmitter::configGraphicGenerics(
            &testMsg.graphicData,
            "\x00\x00\x01",
            RefSerialData::Tx::GraphicOperation::GRAPHIC_ADD,
            1,
            RefSerialData::Tx::GraphicColor::YELLOW);
        RefSerialTransmitter::configLine(4, 100, 100, 200, 200, &testMsg.graphicData);
    }

    RefSerialData::Tx::Graphic1Message testMsg;

    modm::ResumableResult<bool> TestIndicator::sendInitialGraphics()
    {
        RF_BEGIN(0);

        RF_CALL(refSerialTransmitter.sendGraphic(&testMsg));

        RF_END();
    }
}