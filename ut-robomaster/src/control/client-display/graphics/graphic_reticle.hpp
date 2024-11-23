#pragma once

#include "graphic_abstract.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

using subsystems::turret::TurretSubsystem;
namespace graphic
{

class graphic_reticle : public graphic_abstract
{
public:
    graphic_reticle(src::Drivers *drivers, TurretSubsystem *turret)
        : graphic_abstract(drivers, 2),
          turret(turret) {};
    void initialize() override
    {
        // restart();

        RefSerialTransmitter::configGraphicGenerics(
            &msg.graphicData,
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::PINK);

        // RESOLUTION HAS TO BE 1920x1080 OR HUD WILL NOT WORK PROPERLY
        RefSerialTransmitter::configCircle(10, CENTER_X, CENTER_Y, CIRCLE_SIZE, &msg.graphicData);
    };

    modm::ResumableResult<bool> run() override
    {
        RF_BEGIN();

        RF_WAIT_UNTIL(drivers->refSerial.getRefSerialReceivingData());

        // setup new graphic (GRAPHIC_ADD operation)
        RF_CALL(refSerialTransmitter.sendGraphic(&msg));

        while (true)
        {
            // msg.graphicData.operation = RefSerialData::Tx::GRAPHIC_MODIFY;
            // msg.graphicData.lineWidth = 5.0f + 25.0f * t;
            // msg.graphicData.radius = 100.0f + 300.0f * t;
            // DROP_DISTANCE = this.turret.getBulletDropReticle();
            msg.graphicData.startY = CENTER_Y - turret->getBulletDropReticle();
            ;  // startY is centerY

            // modify existing graphic based on the ID (GRAPHIC_MODIFY operation)
            RF_CALL(refSerialTransmitter.sendGraphic(&msg));
        }
        RF_END_RETURN(true);
    };

private:
    static constexpr uint16_t CENTER_X = 1920 / 2;
    static constexpr uint16_t CENTER_Y = 1080 / 2;
    static constexpr uint16_t CIRCLE_SIZE = 10;
    TurretSubsystem *turret;
};

}  // namespace graphic