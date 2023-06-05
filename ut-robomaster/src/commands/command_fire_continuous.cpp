#include "command_fire_continuous.hpp"

namespace commands
{
using subsystems::shooter::FiringState;
using tap::communication::serial::Remote;

void CommandFireContinuous::initialize() {}

void CommandFireContinuous::execute()
{
    Remote::SwitchState switchState = drivers->remote.getSwitch(Remote::Switch::LEFT_SWITCH);

    FiringState firingState = FiringState::Idle;

    switch (switchState)
    {
        case Remote::SwitchState::UP:
            firingState = FiringState::Firing;
            break;
        case Remote::SwitchState::MID:
            firingState = FiringState::Ready;
            break;
        case Remote::SwitchState::DOWN:
        case Remote::SwitchState::UNKNOWN:
            break;
    }

    shooter->setFiringState(firingState);
}

void CommandFireContinuous::end(bool) {}

bool CommandFireContinuous::isFinished() const { return false; }
}  // namespace command