#include "encoder.hpp"

// typedef GpioB9 Sda;
// typedef GpioB8 Scl;
// typedef I2cMaster1 MyI2cMaster;
// modm::Ssd1306<MyI2cMaster> display;

// int func()
// {
// 	Board::initialize();

// 	MyI2cMaster::connect<Scl::Scl, Sda::Sda>();
// 	MyI2cMaster::initialize<Board::SystemClock, 420_kHz>();

// 	display.initializeBlocking();
// 	display.setFont(modm::font::Assertion);
// 	display << "Hello World!";
// 	display.update();

// 	modm::ShortPeriodicTimer timer(1s);
// 	uint16_t counter(0);

// 	while (true)
// 	{
// 		if (timer.execute())
// 		{
// 			display.setCursor(0,20);
// 			display << counter++;
// 			display.update();
// 		}
// 	}

// 	return 0;
// }