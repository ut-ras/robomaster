#include "command_move_turret_aimbot.hpp"

#include "tap/algorithms/ballistics.hpp"

namespace commands
{
using namespace tap::algorithms::ballistics;
using communication::TurretData;

void CommandMoveTurretAimbot::initialize() {}

void CommandMoveTurretAimbot::execute()
{
    // only run if the CV board is online
    if (!drivers->cvBoard.isOnline()) return;

    // only run when there's new data
    if (lastTurretDataIndex == drivers->cvBoard.turretDataIndex) return;
    lastTurretDataIndex = drivers->cvBoard.turretDataIndex;

    // only run if we have a target
    TurretData data = drivers->cvBoard.getTurretData();
    if (!data.hasTarget) return;

    // Pitch axis relative (y/z flipped)
    Vector3f targetPos(
        data.xPos + CAMERA_X_OFFSET,
        data.zPos + CAMERA_TO_PITCH,
        data.yPos + CAMERA_TO_BARRELS);
    Vector3f targetVel(data.xVel, data.zVel, data.yVel);
    Vector3f targetAcc(data.xAcc, data.zAcc, data.yAcc);

    if (USE_BALLISTICS) // use ballistics to adjust turret pitch
    {
        // Rotate to world relative pitch
        float a = turret->getCurrentLocalPitch();
        const float matData[9] = {1.0f, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a)};
        modm::Matrix3f rotMat(matData);
        targetPos = rotMat * targetPos;
        targetVel = rotMat * targetVel;
        targetAcc = rotMat * targetAcc;

        MeasuredKinematicState kinState{targetPos, targetVel, targetAcc};

        float turretPitch = 0.0f;
        float turretYaw = 0.0f;
        float travelTime = 0.0f;

        bool validBallistcs = findTargetProjectileIntersection(
            kinState,
            TARGET_PROJECTILE_VELOCITY,
            BALLISTIC_ITERATIONS,
            &turretPitch,
            &turretYaw,
            &travelTime,
            -NOZZLE_TO_PITCH);

        if (validBallistcs)
        {
            float currentWorldYaw = turret->getCurrentLocalYaw() + turret->getChassisYaw();
            turret->setTargetWorldAngles(currentWorldYaw + turretYaw, turretPitch);
        }
    }
    else // auto aim
    { 
        // find the angle error based on target position from vision
        float deltaYaw = -atan(targetPos.x / targetPos.y);  // yaw is opposite to camera X
        float deltaPitch = atan(targetPos.z / targetPos.y);
        float scale = 0.006f;
        // float currentWorldYaw = getCurrentLocalYaw() + getChassisYaw();
        // float currentWorldPitch = getCurrentLocalPitch();
        turret->setTargetWorldAngles(
            turret->getTargetWorldYaw() + deltaYaw * scale,
            turret->getTargetWorldPitch() + deltaPitch * scale);
    }
}

void CommandMoveTurretAimbot::end(bool) {}

bool CommandMoveTurretAimbot::isFinished(void) const { return false; }
}  // namespace commands
