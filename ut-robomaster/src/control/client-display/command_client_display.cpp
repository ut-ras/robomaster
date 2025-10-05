#include "command_client_display.hpp"

#define PT_AWAIT_RESUMABLE(expr)                                      \
    do                                                                \
    {                                                                 \
        this->ptState = __LINE__;                                     \
        modm_fallthrough;                                             \
        case __LINE__:                                                \
        {                                                             \
            auto _rf = (expr);                                        \
            if (_rf.getState() > modm::rf::NestingError) return true; \
            _rf.getResult(); /* void OK */                            \
        }                                                             \
    } while (0)
/*

Caleb's Beyblade indicator
*/
modm::ResumableResult<void> BeybladeIndicator::sendInitialGraphics()
{
    // The number represents the index of the resumable function in this class

    return refSerialTransmitter.sendGraphic(&msg);
}

modm::ResumableResult<void> BeybladeIndicator::update()
{
    // This is the second resumable function so its index is 1

    return refSerialTransmitter.sendGraphic(&msg);
}

void BeybladeIndicator::initialize()
{
    RefSerialTransmitter::configGraphicGenerics(
        &msg.graphicData[0],
        graphicName,
        RefSerialData::Tx::GRAPHIC_ADD,
        1,                                       // Graphic layer can be 0-9
        RefSerialData::Tx::GraphicColor::PINK);  // is there a way to change this \/\/ to be a tank
                                                 // kind of thing
    RefSerialTransmitter::configCircle(50, 1920 / 2, 1080 / 2, 200, &msg.graphicData[0]);
}

/*
George's Flywheel indicator
*/

modm::ResumableResult<void> FlywheelIndicator::sendInitialGraphics()
{
    // The number represents the index of the resumable function in this class

    return refSerialTransmitter.sendGraphic(&msg);
}

modm::ResumableResult<void> FlywheelIndicator::update()
{
    // This is the second resumable function so its index is 1

    return refSerialTransmitter.sendGraphic(&msg);
}

void FlywheelIndicator::initialize()
{
    RefSerialTransmitter::configGraphicGenerics(
        &msg.graphicData[0],
        graphicName,
        RefSerialData::Tx::GRAPHIC_ADD,
        1,  // Graphic layer can be 0-9
        RefSerialData::Tx::GraphicColor::GREEN);

    // ADJUST APPEARANCE \/ \/
    RefSerialTransmitter::configCircle(50, 1920 / 2, 1080 / 2, 200, &msg.graphicData[0]);
}

namespace commands
{

void CommandClientDisplay::restartHud()
{
    beybladeIndicator.initialize();
    flywheelIndicator.initialize();
    this->restarting = false;
}

bool CommandClientDisplay::run()
{
    if (!this->isRunning())
    {
        restart();
        this->restartHud();
    }

    PT_BEGIN();

    PT_WAIT_UNTIL(drivers->refSerial.getRefSerialReceivingData());

    // Initial draws
    PT_AWAIT_RESUMABLE(beybladeIndicator.sendInitialGraphics());
    PT_AWAIT_RESUMABLE(flywheelIndicator.sendInitialGraphics());

    while (!this->restarting)
    {
        PT_AWAIT_RESUMABLE(beybladeIndicator.update());
        PT_AWAIT_RESUMABLE(flywheelIndicator.update());
        PT_YIELD();
    }

    PT_END();
}
}  // namespace commands
