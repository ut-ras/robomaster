#include "command_move_turret.hpp"

namespace commands
{
void CommandMoveTurret::initialize() { isCalibrated = false; }

void CommandMoveTurret::execute()
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

        // Mouse input
        yawInput = remote->getMouseX() * MOUSE_SENS_YAW * DT;
        pitchInput = -remote->getMouseY() * MOUSE_SENS_PITCH * DT;

        // If no mouse input, get joystick input
        if (yawInput == 0.0f && pitchInput == 0.0f)
        {
            // Joystick input
            float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
            float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);

            if (abs(h) < ANALOG_DEAD_ZONE) h = 0.0f;
            if (abs(v) < ANALOG_DEAD_ZONE) v = 0.0f;

            yawInput = h * abs(h) * YAW_INPUT_SCALE * DT;      // quadratic input map
            pitchInput = v * abs(v) * PITCH_INPUT_SCALE * DT;  // quadratic input map
        }

        yaw -= yawInput;
        pitch += pitchInput;
        pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);

        turret->setTargetWorldAngles(yaw, pitch);
    }
}

void CommandMoveTurret::end(bool) {}

bool CommandMoveTurret::isFinished(void) const { return false; }
}  // namespace commands
