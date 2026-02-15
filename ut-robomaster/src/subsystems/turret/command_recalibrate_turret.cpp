#include "command_recalibrate_turret.hpp"

namespace commands
{

void CommandRecalibrateTurret::initialize() {}

void CommandRecalibrateTurret::execute()
{
    turret->setBaseYaw(turret->getMotorAngle());
    turret->setTargetWorldAngles(
        turret->getCurrentLocalYaw() + turret->getChassisYaw(),
        turret->getCurrentLocalPitch());
}

void CommandRecalibrateTurret::end(bool) {}

bool CommandRecalibrateTurret::isFinished() const { return false; }
}  // namespace commands