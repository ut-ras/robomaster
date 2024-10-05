#include "command_move_turret_mouse.hpp"

namespace commands
{
void CommandMoveTurretMouse::initialize() { isCalibrated = false; }

void CommandMoveTurretMouse::execute()
{
    if (!isCalibrated && turret->getIsCalibrated())
    {
        yaw = turret->getCurrentLocalYaw() + turret->getChassisYaw();
        pitch = turret->getCurrentLocalPitch();
        isCalibrated = true;
    }

    if (isCalibrated)
    {
        Remote* remote = &drivers->remote;
        float yawInput = 0.0f;
        float pitchInput = 0.0f;

        yawInput = remote->getMouseX() * MOUSE_SENS_YAW * DT;
        pitchInput = -remote->getMouseY() * MOUSE_SENS_PITCH * DT;

        yaw -= yawInput;
        pitch += pitchInput;
        pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);

        turret->setTargetWorldAngles(yaw, pitch);

        // can figure out linear_interpolate in the future
    }
}

void CommandMoveTurretMouse::end(bool) {}

bool CommandMoveTurretMouse::isFinished(void) const { return false; }
}  // namespace commands
