#include "encoder.hpp"

#include "tap/architecture/periodic_timer.hpp"

#include "encoder_test.hpp"

using tap::arch::PeriodicMilliTimer;

typedef modm::platform::I2cMaster2 Master;

uint16_t frequencies[360];
typedef GpioF0 Sda;
typedef GpioF1 Scl;

void populate_frequencies(void);

int func()
{
    Board::initialize();
    encoder::Encoder<> encoder;
    Master::connect<Scl::Scl, Sda::Sda>();
    Master::initialize<Board::SystemClock>();
    populate_frequencies();

    // encoder.initializeBlocking();
    encoder.set_startAngle(10);

    PeriodicMilliTimer timer(1000);  // 1 second

    while (true)
    {
        if (timer.execute())
        {
            // uint16_t angle = encoder.read_Angle();
        }
    }

    return 0;
}

void populate_frequencies()
{
    frequencies[0] = 220;
    for (uint16_t i = 1; i < 360; i++)
    {
        frequencies[i] = frequencies[i - 1] + 5;
    }
}
