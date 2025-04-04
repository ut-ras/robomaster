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
            RefSerialData::Tx::GraphicColor::GREEN);

        RefSerialTransmitter::configFloatingNumber(
            100,
            3,
            10,
            700,
            400,
            turret->getChassisYaw(),
            &multiMsg.graphicData[0]);

        graphicId[0] = 5;

        RefSerialTransmitter::configGraphicGenerics(
            &multiMsg.graphicData[1],
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::GREEN);

        RefSerialTransmitter::configFloatingNumber(
            100,
            3,
            10,
            700,
            700,
            turret->getCurrentLocalYaw(),
            &multiMsg.graphicData[1]);


        RefSerialTransmitter::configGraphicGenerics(
            &multiMsg.graphicData[2],
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::GREEN);

        RefSerialTransmitter::configLine(
            100,
            3,
            10,
            700,
            700,
            &multiMsg.graphicData[2]);


        // RefSerialTransmitter::configLine(10, 0, 0, 1920, 1080, &msg.graphicData);
    };

    modm::ResumableResult<void> initialize() override
    {
        RF_BEGIN();
        RF_CALL(refSerialTransmitter.sendGraphic(&multiMsg));
        RF_END_RETURN();
    };

    modm::ResumableResult<bool> run() override
    {
        RF_BEGIN();

        multiMsg.graphicData[0].operation = RefSerialData::Tx::GRAPHIC_MODIFY;
        multiMsg.graphicData[1].operation = RefSerialData::Tx::GRAPHIC_MODIFY;

        multiMsg.graphicData[0].value =  turret->getChassisYaw() * 1000;
        multiMsg.graphicData[1].value =  turret->getCurrentLocalYaw() * 1000 + PI; //turret yaw

        


        // modify existing graphic based on the ID (GRAPHIC_MODIFY operation)
        RF_CALL(refSerialTransmitter.sendGraphic(&multiMsg));

        RF_END_RETURN(true);
    };

private:
    TurretSubsystem *turret;
};

}  // namespace graphic