#include "tap/architecture/periodic_timer.hpp"
#include "tap/architecture/profiler.hpp"
#include "tap/motor/motorsim/dji_motor_sim_handler.hpp"

#include "robots/robot_constants.hpp"
#include "robots/robot_control.hpp"

#include "board.hpp"
#include "drivers.hpp"

// Place any sort of input/output initialization here. For example, place
// serial init stuff here.
static void initializeIo(src::Drivers* drivers)
{
    drivers->analog.init();
    drivers->pwm.init();
    drivers->digital.init();
    drivers->leds.init();
    drivers->can.initialize();
    drivers->errorController.init();
    drivers->remote.initialize();
    // drivers->refSerial.initialize();
    drivers->cvBoard.initialize();
    drivers->terminalSerial.initialize();
    drivers->schedulerTerminalHandler.init();
    drivers->djiMotorTerminalSerialHandler.init();
    drivers->bmi088.initialize(IMU_SAMPLE_FREQUENCY, IMU_KP, IMU_KI);

    // vvv Original code before taproot update
    // drivers->bmi088.requestRecalibration();
    drivers->bmi088.requestCalibration();
}

// Anything that you would like to be called place here. It will be called
// very frequently. Use PeriodicMilliTimers if you don't want something to be
// called as frequently.
static void updateIo(src::Drivers* drivers)
{
#ifdef PLATFORM_HOSTED
    tap::motor::motorsim::DjiMotorSimHandler::getInstance()->updateSims();
#endif

    drivers->canRxHandler.pollCanData();
    // drivers->refSerial.updateSerial();
    drivers->cvBoard.updateSerial();
    drivers->remote.read();
}

static void updateImu(src::Drivers* drivers)
{
    drivers->bmi088.read();
    drivers->bmi088.periodicIMUUpdate();
}

src::Drivers drivers;
RobotControl control{&drivers};

int main()
{
    Board::initialize();
    initializeIo(&drivers);
    Board::initialize_i2c();
    control.initialize();

    tap::arch::PeriodicMilliTimer refreshTimer(REFRESH_PERIOD);

    while (1)
    {
        // do this as fast as you can
        PROFILE(drivers.profiler, updateIo, (&drivers));

        if (refreshTimer.execute())
        {
            PROFILE(drivers.profiler, updateImu, (&drivers));
            PROFILE(drivers.profiler, drivers.commandScheduler.run, ());
            PROFILE(drivers.profiler, drivers.djiMotorTxHandler.encodeAndSendCanData, ());
            PROFILE(drivers.profiler, drivers.terminalSerial.update, ());
        }

        // prevent looping too fast
        modm::delay_us(10);
    }
    return 0;
}
