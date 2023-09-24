#include "command_move_turret_joystick.hpp"

#define ANALOG_DEAD_ZONE 0.1

namespace commands
{
void CommandMoveTurretJoystick::initialize()
{
    yaw = turret->getTargetWorldYaw();
    pitch = turret->getTargetWorldPitch();
}

void CommandMoveTurretJoystick::execute()
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

        float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
        float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);

        if (abs(h) < ANALOG_DEAD_ZONE) h = 0.0f;
        if (abs(v) < ANALOG_DEAD_ZONE) v = 0.0f;

        yawInput = h * abs(h);    // quadratic input map
        pitchInput = v * abs(v);  // quadratic input map

        yaw -= yawInput * YAW_INPUT_SCALE;
        pitch += pitchInput * PITCH_INPUT_SCALE;
        pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);
    }

    turret->setTargetWorldAngles(yaw, pitch);
}

void CommandMoveTurretJoystick::end(bool) {}

bool CommandMoveTurretJoystick::isFinished(void) const { return false; }
}  // namespace commands
