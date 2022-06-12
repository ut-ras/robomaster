#include "agitator_rotate_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"
namespace control
{
namespace agitator
{
void AgitatorRotateCommand::initialize() { isUnjamming.stop(); }

void AgitatorRotateCommand::execute() { 
    if (isUnjamming.isStopped())
    {
        if (agitator->getAgitatorMotor().getTorque() > jamTorque)
        {
            isUnjamming.restart(1000);
        }

        else
        {
            agitator->setDesiredRPM(rotateRPM); 
        }
    }

    else
    {
        agitator->setDesiredRPM(-rotateRPM);
    }
    }

void AgitatorRotateCommand::end(bool) { agitator->setDesiredRPM(0); }

bool AgitatorRotateCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
