#include "command_client_display.hpp"

namespace commands
{

void CommandClientDisplay::initialize()
{
    tap::buzzer::playNote(&drivers->pwm, 440);
    // restart();

    // RefSerialTransmitter::configGraphicGenerics(
    //     &msg.graphicData,
    //     graphicId,
    //     RefSerialData::Tx::GRAPHIC_ADD,
    //     0,
    //     RefSerialData::Tx::GraphicColor::PINK);

    // // RESOLUTION HAS TO BE 1920x1080 OR HUD WILL NOT WORK PROPERLY
    // RefSerialTransmitter::configCircle(10, 1920 / 2, 1080 / 2, 100, &msg.graphicData);
    // std::vector<graphic::graphic_abstract*> currentGraphics = client->getGraphics();
    // for (graphic::graphic_abstract* graphic : currentGraphics)
    // {
    //     graphic->initialize();
    // }

    graphic = client->getGraphics();
    graphic->initialize();
}

void CommandClientDisplay::execute() { run(); }

void CommandClientDisplay::end(bool)
{
    tap::buzzer::silenceBuzzer(&drivers->pwm);
}

bool CommandClientDisplay::isFinished() const { return false; }

bool CommandClientDisplay::run()
{
    // float t = sinf(tap::arch::clock::getTimeMilliseconds() / 1000.0f * 4.0f) * 0.5f + 0.5f;
    // graphic = client->getGraphics();

    PT_BEGIN();

    // PT_WAIT_UNTIL(drivers->refSerial.getRefSerialReceivingData());

    // // setup new graphic (GRAPHIC_ADD operation)
    // PT_CALL(refSerialTransmitter.sendGraphic(&msg));

    // while (true)
    // {
    //     msg.graphicData.operation = RefSerialData::Tx::GRAPHIC_MODIFY;
    //     msg.graphicData.lineWidth = 5.0f + 25.0f * t;
    //     msg.graphicData.radius = 100.0f + 300.0f * t;

    //     // modify existing graphic based on the ID (GRAPHIC_MODIFY operation)
    //     PT_CALL(refSerialTransmitter.sendGraphic(&msg));
    // }

    // PT_END();
    // std::vector<graphic::graphic_abstract*> currentGraphics = client->getGraphics();
    // for (graphic::graphic_abstract* graphic : currentGraphics)
    // {
    //     graphic->run();
    // }
    PT_CALL(graphic->run());

    PT_END();
}
}  // namespace commands