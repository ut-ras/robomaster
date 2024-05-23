#ifdef PLATFORM_HOSTED
/* hosted environment (simulator) includes --------------------------------- */
#include <iostream>

#include "tap/communication/tcp-server/tcp_server.hpp"
#include "tap/motor/motorsim/sim_handler.hpp"
#endif

#include "tap/architecture/periodic_timer.hpp"
#include "tap/architecture/profiler.hpp"

#include "robots/robot_constants.hpp"
#include "robots/robot_control.hpp"

#include "board.hpp"
#include "drivers.hpp"

// Place any sort of input/output initialization here. For example, place
// serial init stuff here.
static void initializeIo(src::Drivers *drivers)
{
    drivers->analog.init();
    drivers->pwm.init();
    drivers->digital.init();
    drivers->leds.init();
    drivers->can.initialize();
    drivers->errorController.init();
    drivers->remote.initialize();
    drivers->refSerial.initialize();
    // drivers->cvBoard.initialize();
    drivers->terminalSerial.initialize();
    drivers->schedulerTerminalHandler.init();
    drivers->djiMotorTerminalSerialHandler.init();
    drivers->bmi088.initialize(IMU_SAMPLE_FREQUENCY, IMU_KP, IMU_KI);
    drivers->bmi088.requestRecalibration();
}

// Anything that you would like to be called place here. It will be called
// very frequently. Use PeriodicMilliTimers if you don't want something to be
// called as frequently.
static void updateIo(src::Drivers *drivers)
{
#ifdef PLATFORM_HOSTED
    tap::motorsim::SimHandler::updateSims();
#endif

    drivers->canRxHandler.pollCanData();
    drivers->refSerial.updateSerial();
    // drivers->cvBoard.updateSerial();
    drivers->remote.read();
}

int main()
{
#ifdef PLATFORM_HOSTED
    std::cout << "Simulation starting..." << std::endl;
#endif

    src::Drivers drivers;

    Board::initialize();
    initializeIo(&drivers);
    Board::initialize_i2c();

    Control{&drivers};

#ifdef PLATFORM_HOSTED
    tap::motorsim::SimHandler::resetMotorSims();
    // Blocking call, waits until Windows Simulator connects.
    tap::communication::TCPServer::MainServer()->getConnection();
#endif

    tap::arch::PeriodicMilliTimer refreshTimer(REFRESH_PERIOD);

    while (1)
    {
        // do this as fast as you can
        PROFILE(drivers.profiler, updateIo, (&drivers));

        if (refreshTimer.execute())
        {
            PROFILE(drivers.profiler, drivers.bmi088.periodicIMUUpdate, ());
            PROFILE(drivers.profiler, drivers.commandScheduler.run, ());
            PROFILE(drivers.profiler, drivers.djiMotorTxHandler.encodeAndSendCanData, ());
            PROFILE(drivers.profiler, drivers.terminalSerial.update, ());
        }

        // prevent looping too fast
        modm::delay_us(10);
    }
    return 0;
}