#pragma once
#include "graphic_abstract.hpp"

namespace graphic
{

class graphic_circle : public graphic_abstract
{
public:
    graphic_circle(src::Drivers *drivers) : graphic_abstract(drivers)
    {
        graphicId[0] = 0;

        RefSerialTransmitter::configGraphicGenerics(
            &msg.graphicData,
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::PINK);

        RefSerialTransmitter::configCircle(10, 1920 / 2, 1080 / 2, 100, &msg.graphicData);
    };

    modm::ResumableResult<void> initialize() override
    {
        RF_BEGIN();
        RF_CALL(refSerialTransmitter.sendGraphic(&msg));
        RF_END_RETURN();
    };

    modm::ResumableResult<bool> run() override
    {
        float t = sinf(tap::arch::clock::getTimeMilliseconds() / 1000.0f * 4.0f) * 0.5f + 0.5f;
        RF_BEGIN();

        msg.graphicData.operation = RefSerialData::Tx::GRAPHIC_MODIFY;
        msg.graphicData.lineWidth = 5.0f + 25.0f * t;
        msg.graphicData.radius = 100.0f + 300.0f * t;

        // modify existing graphic based on the ID (GRAPHIC_MODIFY operation)
        RF_CALL(refSerialTransmitter.sendGraphic(&msg));

        RF_END_RETURN(true);
    };

private:
    RefSerialData::Tx::Graphic1Message msg;
    float t = 10.0;
};

}  // namespace graphic