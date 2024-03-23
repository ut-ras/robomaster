#include "command_agitator_continuous.hpp"

namespace commands
{


void CommandAgitatorContinuous::initialize() {
    timeout.restart(1000E-10);
}

void CommandAgitatorContinuous::execute()
{
    float bps = BALLS_PER_SEC;
    unsigned long time;
    bool unjam;

    if (drivers->refSerial.getRefSerialReceivingData() &&
        power_limiter::getRemainingCooldown(drivers, barrelId) <= BARREL_HEAT_BUFFER)
    {
        bps = 0.0f;
    }


   if ( agitator->getVelocity() < (.5*BALLS_PER_SEC/BALLS_PER_REV) )  {

    if (timeout.isExpired()){
        timeout2.restart(1000E-10);
        unjam = true;
    } 
    
} else {
    timeout.restart(1000E-10);
}

if (timeout2.isExpired()) { unjam = false; }

if (unjam) {bps = UNJAM_SPEED;}

agitator->setBallsPerSecond(bps);

}

void CommandAgitatorContinuous::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandAgitatorContinuous::isFinished() const { return false; }
}  // namespace commands