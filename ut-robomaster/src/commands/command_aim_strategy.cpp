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

        if(remote->getMouseR()) // Any concerns for holding right click for too long?
        {
            switch (turret->getAimStrategy())
            {
            case subsystems::turret::AimStrategy::Manual:
                turret->setAimStrategy(subsystems::turret::AimStrategy::AutoAim);
                break;
            case subsystems::turret::AimStrategy::AutoAim:
                turret->setAimStrategy(subsystems::turret::AimStrategy::Manual);
                break;
            default:
                break;
            }
        }
    }

    void CommandAimStrategy::end(bool) {}
    bool CommandAimStrategy::isFinished() const { return false; }
}  // namespace commands