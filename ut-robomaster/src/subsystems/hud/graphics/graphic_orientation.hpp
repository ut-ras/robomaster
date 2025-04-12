#pragma once
#include "subsystems/turret/turret_subsystem.hpp"

#include "graphic_abstract.hpp"
using subsystems::turret::TurretSubsystem;

namespace graphic
{

class graphic_orientation : public graphic_abstract
{
public:
    graphic_orientation(src::Drivers *drivers, TurretSubsystem *turret)
        : graphic_abstract(drivers),
          turret(turret)
    {
        graphicId[0] = 4;

        RefSerialTransmitter::configGraphicGenerics(
            &multiMsg.graphicData[0],
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::ORANGE);

        graphicId[0] = 5;

        RefSerialTransmitter::configGraphicGenerics(
            &multiMsg.graphicData[1],
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::GREEN);

        graphicId[0] = 6;

        RefSerialTransmitter::configGraphicGenerics(
            &multiMsg.graphicData[2],
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::ORANGE);

        graphicId[0] = 7;

        RefSerialTransmitter::configGraphicGenerics(
            &multiMsg.graphicData[3],
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::ORANGE);

        RefSerialTransmitter::configLine(10, 0, 0, 1920, 1080, &multiMsg.graphicData[0]);
        RefSerialTransmitter::configLine(10, 0, 0, 1920, 1080, &multiMsg.graphicData[1]);
        RefSerialTransmitter::configLine(10, 0, 0, 1920, 1080, &multiMsg.graphicData[2]);
        RefSerialTransmitter::configLine(10, 0, 0, 1920, 1080, &multiMsg.graphicData[3]);
    };

    modm::ResumableResult<void> initialize() override
    {
        RF_BEGIN();
        RF_CALL(refSerialTransmitter.sendGraphic(&multiMsg));
        RF_END_RETURN();
    };

    modm::ResumableResult<bool> run() override
    {
        float a = -turret->getCurrentLocalYaw();
        float r = 100.0f;

        float mx = 150.0f;
        float my = 700.0f;

        float ax = mx + cosf(a - M_PI_4) * r;
        float ay = my + sinf(a - M_PI_4) * r;
        float bx = mx + cosf(a + M_PI_4) * r;
        float by = my + sinf(a + M_PI_4) * r;
        float cx = mx + cosf(a - M_PI_4 + M_PI) * r;
        float cy = my + sinf(a - M_PI_4 + M_PI) * r;
        float dx = mx + cosf(a + M_PI_4 + M_PI) * r;
        float dy = my + sinf(a + M_PI_4 + M_PI) * r;

        RF_BEGIN();

        multiMsg.graphicData[0].operation = RefSerialData::Tx::GRAPHIC_MODIFY;
        multiMsg.graphicData[1].operation = RefSerialData::Tx::GRAPHIC_MODIFY;
        multiMsg.graphicData[2].operation = RefSerialData::Tx::GRAPHIC_MODIFY;
        multiMsg.graphicData[3].operation = RefSerialData::Tx::GRAPHIC_MODIFY;

        RefSerialTransmitter::configLine(10, ax, ay, bx, by, &multiMsg.graphicData[0]);
        RefSerialTransmitter::configLine(10, bx, by, cx, cy, &multiMsg.graphicData[1]);
        RefSerialTransmitter::configLine(10, cx, cy, dx, dy, &multiMsg.graphicData[2]);
        RefSerialTransmitter::configLine(10, dx, dy, ax, ay, &multiMsg.graphicData[3]);

        // msg.graphicData[0].value = turret->getChassisYaw() * 1000;
        // msg.graphicData[1].value = turret->getCurrentLocalYaw() * 1000 + PI;  // turret yaw

        // modify existing graphic based on the ID (GRAPHIC_MODIFY operation)
        RF_CALL(refSerialTransmitter.sendGraphic(&multiMsg));

        RF_END_RETURN(true);
    };

private:
    TurretSubsystem *turret;
    RefSerialData::Tx::Graphic5Message multiMsg;
};

}  // namespace graphic