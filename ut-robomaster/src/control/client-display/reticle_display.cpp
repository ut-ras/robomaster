#include "reticle_display.hpp"

namespace commands
{

void reticle_display::initialize()
{
    restart();

    RefSerialTransmitter::configGraphicGenerics(
        &msg.graphicData,
        graphicId,
        RefSerialData::Tx::GRAPHIC_ADD,
        0,
        RefSerialData::Tx::GraphicColor::PINK);

    // RESOLUTION HAS TO BE 1920x1080 OR HUD WILL NOT WORK PROPERLY
    RefSerialTransmitter::configCircle(
        10, CENTER_X, 
        CENTER_Y, 
        CIRCLE_SIZE,
        &msg.graphicData);
}

void reticle_display::execute() { run(); }

void reticle_display::end(bool) { }//tap::buzzer::silenceBuzzer(&drivers->pwm); }

bool reticle_display::isFinished() const { return false; }

bool reticle_display::run()
{
    //float t = sinf(tap::arch::clock::getTimeMilliseconds() / 1000.0f * 4.0f) * 0.5f + 0.5f;

    PT_BEGIN();

    PT_WAIT_UNTIL(drivers->refSerial.getRefSerialReceivingData());

    // setup new graphic (GRAPHIC_ADD operation)
    PT_CALL(refSerialTransmitter.sendGraphic(&msg));

    while (true)
    {
        // msg.graphicData.operation = RefSerialData::Tx::GRAPHIC_MODIFY;
        // msg.graphicData.lineWidth = 5.0f + 25.0f * t;
        // msg.graphicData.radius = 100.0f + 300.0f * t;
        // DROP_DISTANCE = this.turret.getBulletDropReticle();
        msg.graphicData.startY = CENTER_Y-turret->getBulletDropReticle();; //startY is centerY

        // modify existing graphic based on the ID (GRAPHIC_MODIFY operation)
        PT_CALL(refSerialTransmitter.sendGraphic(&msg));
    }

    PT_END();
}
}  // namespace commands