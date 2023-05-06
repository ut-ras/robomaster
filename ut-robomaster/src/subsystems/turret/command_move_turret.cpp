#include "command_move_turret.hpp"

#define ANALOG_DEAD_ZONE 0.1

namespace subsystems
{
namespace turret
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
        float h = remote->getChannel(Remote::Channel::LEFT_HORIZONTAL);
        float v = remote->getChannel(Remote::Channel::LEFT_VERTICAL);

        if (abs(h) < ANALOG_DEAD_ZONE) h = 0.0f;
        if (abs(v) < ANALOG_DEAD_ZONE) v = 0.0f;

        float yawInput = h * abs(h);    // quadratic input map
        float pitchInput = v * abs(v);  // quadratic input map

        yaw -= yawInput * yawInputScale;
        pitch += pitchInput * pitchInputScale;
        pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);
    }

    turret->inputManualAngles(yaw, pitch);
}

void CommandMoveTurret::end(bool) {}

bool CommandMoveTurret::isFinished(void) const { return false; }
}  // namespace turret
}  // namespace subsystems
