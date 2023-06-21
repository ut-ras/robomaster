#include "command_shooter_default.hpp"

namespace commands
{
using tap::communication::serial::Remote;

void CommandShooterDefault::initialize() {}

void CommandShooterDefault::execute()
{
    // Remote::SwitchState switchState = drivers->remote.getSwitch(Remote::Switch::LEFT_SWITCH);

    // FiringState firingState = FiringState::Idle;

    // switch (switchState)
    // {
    //     case Remote::SwitchState::UP:
    //         firingState = FiringState::Firing;
    //         break;
    //     case Remote::SwitchState::MID:
    //         firingState = FiringState::Ready;
    //         break;
    //     case Remote::SwitchState::DOWN:
    //     case Remote::SwitchState::UNKNOWN:
    //         break;
    // }

    // // Always stay ready in keyboard mode
    // if (drivers->remote.getSwitch(Remote::Switch::RIGHT_SWITCH) == Remote::SwitchState::DOWN)
    // {
    //     firingState = FiringState::Ready;
    // }

    // shooter->setFiringState(firingState);
}

void CommandShooterDefault::end(bool) {}

bool CommandShooterDefault::isFinished() const { return false; }
}  // namespace commands