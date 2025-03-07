#include "command_client_display.hpp"

namespace commands
{

void CommandClientDisplay::initialize()
{
    tap::buzzer::playNote(&drivers->pwm, 440);

    circle = client->getCircle();
    reticle = client->getReticle();
    flywheel_on = client->getFlywheelOn();
    
    circle->initialize();
    reticle->initialize();
    flywheel_on->initialize();
}

void CommandClientDisplay::execute() { run(); }

void CommandClientDisplay::end(bool)
{
    tap::buzzer::silenceBuzzer(&drivers->pwm);
}

bool CommandClientDisplay::isFinished() const { return false; }

bool CommandClientDisplay::run()
{
    PT_BEGIN();

    // PT_CALL(circle->run());
    PT_CALL(reticle->run());
    PT_CALL(flywheel_on->run());

    PT_END();
}
}  // namespace commands