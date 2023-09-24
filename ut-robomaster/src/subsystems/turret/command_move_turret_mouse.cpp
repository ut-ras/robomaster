#include "command_move_turret_mouse.hpp"

namespace commands
{
void CommandMoveTurretMouse::initialize()
{
    yaw = turret->getTargetWorldYaw();
    pitch = turret->getTargetWorldPitch();
}

void CommandMoveTurretMouse::execute()
{
    Remote* remote = &drivers->remote;

    if (drivers->isKillSwitched())
    {
        yaw = turret->getCurrentLocalYaw() + turret->getChassisYaw();
        pitch = turret->getCurrentLocalPitch();
    }

    else
    {
        float yawInput = 0.0f;
        float pitchInput = 0.0f;

        yawInput = remote->getMouseX() * MOUSE_SENS_YAW;
        pitchInput = -remote->getMouseY() * MOUSE_SENS_PITCH;

        yaw -= yawInput;
        pitch += pitchInput;
        pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);
    }

    turret->setTargetWorldAngles(yaw, pitch);
}

void CommandMoveTurretMouse::end(bool) {}

bool CommandMoveTurretMouse::isFinished(void) const { return false; }
}  // namespace commands
