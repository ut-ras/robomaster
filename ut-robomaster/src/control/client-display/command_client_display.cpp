#include "command_client_display.hpp"

modm::ResumableResult<bool> BeybladeIndicator::sendInitialGraphics()
{
	// The number represents the index of the resumable function in this class
	RF_BEGIN(0);
	
	RF_CALL(refSerialTransmitter.sendGraphic(&msg));

	RF_END();
}

modm::ResumableResult<bool> BeybladeIndicator::update()
{
	// This is the second resumable function so its index is 1
	RF_BEGIN(1);
	RF_END();
}

void BeybladeIndicator::initialize()
{
		RefSerialTransmitter::configGraphicGenerics(
			&msg.graphicData[0],
			graphicName,
			RefSerialData::Tx::GRAPHIC_ADD,
			1, // Graphic layer can be 0-9
			RefSerialData::Tx::GraphicColor::PINK
			);
		RefSerialTransmitter::configRectangle(
			100,
			0,
			0,
			100,
			100,
			&msg.graphicData[0]
			);
	}

namespace commands
{

void CommandClientDisplay::restartHud()
{
	beybladeIndicator.initialize();

	this->restarting = false;
}

bool CommandClientDisplay::run()
{
	if (!this->isRunning()) {
		restart();
		this->restartHud();
	}

	PT_BEGIN();
	
	PT_WAIT_UNTIL(drivers.refSerial.getRefSerialReceivingData());

	PT_CALL(beybladeIndicator.sendInitialGraphics());

	while (!this->restarting) {
		PT_CALL(beybladeIndicator.update());
		PT_YIELD();
	}

	PT_END();
}
}
