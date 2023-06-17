#include "command_aim_strategy.hpp"

namespace commands
{
    void CommandAimStrategy::initialize() 
    {
        turret->setAimStrategy(subsystems::turret::AimStrategy::Manual);
    }

    void CommandAimStrategy::execute() 
    {
        Remote* remote = &drivers->remote;
        
        if(remote->getMouseR())
        {
            turret->setAimStrategy(subsystems::turret::AimStrategy::AutoAim);
        }
        else
        {
            turret->setAimStrategy(subsystems::turret::AimStrategy::Manual);
        }
    }

    void CommandAimStrategy::end(bool) {}
    bool CommandAimStrategy::isFinished() const { return false; }
}  // namespace commands