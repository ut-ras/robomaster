// TODO: Make header file for this class
// #include "client_display_command.hpp"

#include "tap/drivers.hpp"
#include "tap/control/command.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "modm/processing/protothread.hpp"
#include "modm/processing/resumable.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

// TODO: Put this indicator in its own files
// Always update the Resumable<T> to match number of functions returning a ResumableResult
class BeybladeIndicator : protected modm::Resumable<2>
{
private:
	const uint8_t* graphicName = (uint8_t*)"\x00\x00\x01";
protected:
	RefSerialTransmitter &refSerialTransmitter;
	RefSerialData::Tx::Graphic5Message msg;

public:
	BeybladeIndicator(RefSerialTransmitter refSerialTransmitter)
	: refSerialTransmitter(refSerialTransmitter)
	{}

	modm::ResumableResult<bool> sendInitialGraphics()
	{
		// The number represents the index of the resumable function in this class
		RF_BEGIN(0);
		
		RF_CALL(refSerialTransmitter.sendGraphic(&msg));

		RF_END();
	}
	modm::ResumableResult<bool> update()
	{
		// This is the second resumable function so its index is 1
		RF_BEGIN(1);
		RF_END();
	}

	void initialize()
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
};

class ClientDisplayCommand : Command, modm::pt::Protothread
{
private:
	tap::Drivers &drivers;
	RefSerialTransmitter refSerialTransmitter;
	BeybladeIndicator beybladeIndicator;
	bool restarting = true;

	void restartHud()
	{
		beybladeIndicator.initialize();

		this->restarting = false;
	}

public:
	ClientDisplayCommand(
		tap::Drivers &drivers)
		: Command(),
		drivers(drivers),
		refSerialTransmitter(&drivers),
		beybladeIndicator(refSerialTransmitter)
	{}
	
	bool run()
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

	const char *getName() const override { return "client display"; }
	void initialize() override
	{
		this->restarting = true;
	};
	void execute() override
	{
		run();
	}
	void end(bool) override {}
	bool isFinished() const override { return false; }
};
