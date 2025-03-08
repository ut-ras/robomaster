#pragma once

#include "subsystems/flywheel/flywheel_subsystem.hpp"

#include "graphic_abstract.hpp"

using subsystems::flywheel::FlywheelSubsystem;
using tap::communication::serial::RefSerialData;
namespace graphic
{

class graphic_flywheel_on : public graphic_abstract
{
public:
    graphic_flywheel_on(src::Drivers *drivers, FlywheelSubsystem *flywheel)
        : graphic_abstract(drivers),
          flywheel(flywheel)
    {
        graphicId[0] = 2;

        RefSerialTransmitter::configGraphicGenerics(
            &word_msg.graphicData,
            graphicId,
            RefSerialData::Tx::GRAPHIC_ADD,
            0,
            RefSerialData::Tx::GraphicColor::ORANGE);

        // RESOLUTION HAS TO BE 1920x1080 OR HUD WILL NOT WORK PROPERLY
        const char *message = "Flywheel on";
        RefSerialTransmitter::configCharacterMsg(50, 10, CENTER_X, CENTER_Y, message, &word_msg);
    };

    modm::ResumableResult<void> initialize() override
    {
        RF_BEGIN();
        RF_CALL(refSerialTransmitter.sendGraphic(&word_msg));
        RF_END_RETURN();
    };

    modm::ResumableResult<bool> run() override
    {
        RF_BEGIN();

        // msg.graphicData.lineWidth = 5.0f + 25.0f * t;
        // msg.graphicData.radius = 100.0f + 300.0f * t;
        // DROP_DISTANCE = this.turret.getBulletDropReticle();
        // msg.graphicData.startY = CENTER_Y - turret->getBulletDropReticle();
        // startY is centerY

        word_msg.graphicData.operation = RefSerialData::Tx::GRAPHIC_MODIFY;

        if (flywheel->isActive())
        {
            RefSerialTransmitter::configCharacterMsg(
                50,
                10,
                CENTER_X,
                CENTER_Y,
                "Flywheel on",
                &word_msg);
        }
        else
        {
            RefSerialTransmitter::configCharacterMsg(
                50,
                10,
                CENTER_X,
                CENTER_Y,
                "Flywheel off",
                &word_msg);
        }

        RF_CALL(refSerialTransmitter.sendGraphic(&word_msg));
        RF_END_RETURN(true);
    };

private:
    static constexpr uint16_t CENTER_X = 1920 / 3;
    static constexpr uint16_t CENTER_Y = 1080 / 3;
    FlywheelSubsystem *flywheel;
};

}  // namespace graphic