#include "command_client_display.hpp"

modm::ResumableResult<bool> BeybladeIndicator::sendInitialGraphics(src::Drivers *drivers)
{
    // The number represents the index of the resumable function in this class
    RF_BEGIN(0);

    tap::buzzer::playNote(&drivers->pwm, 880);
    RF_CALL(refSerialTransmitter.sendGraphic(&msg));

    RF_END();
}

modm::ResumableResult<bool> BeybladeIndicator::update()
{
    // This is the second resumable function so its index is 1
    RF_BEGIN(1);

    RF_CALL(refSerialTransmitter.sendGraphic(&msg));

    RF_END();
}

void BeybladeIndicator::initialize()
{
    RefSerialTransmitter::configGraphicGenerics(
        &msg.graphicData[0],
        graphicName,
        RefSerialData::Tx::GRAPHIC_ADD,
        1,  // Graphic layer can be 0-9
        RefSerialData::Tx::GraphicColor::PINK);
    RefSerialTransmitter::configCircle(10, 400, 400, 200, &msg.graphicData[0]);
}

namespace commands
{

void CommandClientDisplay::initialize()
{
    beybladeIndicator.initialize();
    restart();
}

void CommandClientDisplay::execute() { run(); }

void CommandClientDisplay::end(bool) { tap::buzzer::silenceBuzzer(&drivers->pwm); }

bool CommandClientDisplay::isFinished() const { return !isRunning(); }

bool CommandClientDisplay::run()
{
    PT_BEGIN();

    PT_WAIT_UNTIL(drivers->refSerial.getRefSerialReceivingData());
    PT_CALL(beybladeIndicator.sendInitialGraphics(drivers));

    PT_END();
}
}  // namespace commands