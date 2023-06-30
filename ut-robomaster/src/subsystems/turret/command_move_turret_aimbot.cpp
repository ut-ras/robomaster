#include "command_move_turret_aimbot.hpp"

#include "tap/algorithms/ballistics.hpp"

using namespace tap::algorithms::ballistics;

namespace commands
{
void CommandMoveTurretAimbot::initialize() {
    yaw = turret->getInputYaw();
    pitch = turret->getInputPitch();
}

void CommandMoveTurretAimbot::execute() {
    if (drivers->beagleboneCommunicator.getTurretData().hasTarget) {
        float bulletVelocity = 1.0f;
        uint8_t numBallisticIterations = 1;

        float turretPitch = 0.0f;
        float turretYaw = 0.0f;
        float projectedTravelTime = 0.0f;

        Vector3f targetPosition = Vector3f(0.0f);
        Vector3f targetVelocity = Vector3f(0.0f);
        Vector3f targetAcceleration = Vector3f(0.0f);

        findTargetProjectileIntersection(
            {targetPosition, targetVelocity, targetAcceleration},
            bulletVelocity,
            numBallisticIterations,
            &turretPitch,
            &turretYaw,
            &projectedTravelTime);

        turret->inputManualAngles(turretYaw, turretPitch);      
    }

    else {
        Remote* remote = &drivers->remote;

        if (drivers->isKillSwitched()) {
            yaw = turret->getCurrentLocalYaw() + turret->getChassisYaw();
            pitch = turret->getCurrentLocalPitch();
        }
        
        else {
            float yawInput = 0.0f;
            float pitchInput = 0.0f;

            yawInput = remote->getMouseX() * MOUSE_SENS_YAW;
            pitchInput = -remote->getMouseY() * MOUSE_SENS_PITCH;

            yaw -= yawInput;
            pitch += pitchInput;
            pitch = modm::min(modm::max(pitch, PITCH_MIN), PITCH_MAX);
        }

        turret->inputManualAngles(yaw, pitch);
    }
}

void CommandMoveTurretAimbot::end(bool) {}

bool CommandMoveTurretAimbot::isFinished(void) const { return false; }
}  // namespace commands
