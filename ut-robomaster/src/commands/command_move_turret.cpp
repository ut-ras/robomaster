#include "command_move_turret.hpp"

#include "utils/mouse_tracker.hpp"

#define ANALOG_DEAD_ZONE 0.1

namespace commands
{
void CommandMoveTurret::initialize() {}

void CommandMoveTurret::execute()
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

        if (remote->getSwitch(Remote::Switch::RIGHT_SWITCH) == Remote::SwitchState::DOWN)
        {
            // mouse
            yawInput = remote->getMouseX() * MOUSE_SENS_YAW;
            pitchInput = -remote->getMouseY() * MOUSE_SENS_PITCH;
        }
        else
        {
            // controller
            float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
            float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);

            if (abs(h) < ANALOG_DEAD_ZONE) h = 0.0f;
            if (abs(v) < ANALOG_DEAD_ZONE) v = 0.0f;

            yawInput = h * abs(h);    // quadratic input map
            pitchInput = v * abs(v);  // quadratic input map
        }

        yaw -= yawInput * yawInputScale;
        pitch += pitchInput * pitchInputScale;
        pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);
    }

    float lookBehindYawOffset = state->isLookingBehind ? M_PI : 0.0f;
    turret->inputManualAngles(yaw + lookBehindYawOffset, pitch);
}

void CommandMoveTurret::end(bool) {}

bool CommandMoveTurret::isFinished(void) const { return false; }
}  // namespace commands
