#include "command_client_display.hpp"

namespace commands
{

void CommandClientDisplay::initialize()
{
    tap::buzzer::playNote(&drivers->pwm, 440);

    circle = client->getCircle();
    reticle = client->getReticle();
    flywheel_on = client->getFlywheelOn();
}

void CommandClientDisplay::execute() { run(); }

void CommandClientDisplay::end(bool) { tap::buzzer::silenceBuzzer(&drivers->pwm); }

bool CommandClientDisplay::isFinished() const { return false; }

bool CommandClientDisplay::run()
{
    PT_BEGIN();

    // Initialize
    PT_WAIT_UNTIL(drivers->refSerial.getRefSerialReceivingData());
    PT_CALL(circle->initialize());
    PT_CALL(reticle->initialize());
    PT_CALL(flywheel_on->initialize());

    // Update
    while (true)
    {
        PT_CALL(reticle->run());
        PT_WAIT_UNTIL(hudTimer.execute());
        PT_CALL(circle->run());
        PT_WAIT_UNTIL(hudTimer.execute());
        PT_CALL(flywheel_on->run());
        PT_WAIT_UNTIL(hudTimer.execute());
    }

    PT_END();
}
}  // namespace commands