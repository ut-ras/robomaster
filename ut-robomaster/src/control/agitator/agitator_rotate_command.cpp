#include "agitator_rotate_command.hpp"

#include "tap/control/command.hpp"

#include "agitator_subsystem.hpp"
namespace control
{
namespace agitator
{
void AgitatorRotateCommand::initialize() {
    isUnjamming = false;
    prevTime = tap::arch::clock::getTimeMilliseconds();
}

void AgitatorRotateCommand::execute() { 

    if (!isUnjamming)
    {
        if (agitator->getAgitatorMotor().getTorque() > jamTorque)
        {
            isUnjamming = true;
            prevTime = tap::arch::clock::getTimeMilliseconds();
        }

        else
        {
            agitator->setDesiredRPM(rotateRPM); 
        }
    }

    else
    {
        if (tap::arch::clock::getTimeMilliseconds() - prevTime >= 1000){
            isUnjamming = false;
        }
        agitator->setDesiredRPM(-rotateRPM);
    }
    }

void AgitatorRotateCommand::end(bool) { agitator->setDesiredRPM(0); }

bool AgitatorRotateCommand::isFinished() const { return false; }
}  // namespace agitator
}  // namespace control
