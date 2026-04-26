#include "command_sentry_aim.hpp"

#include "tap/algorithms/ballistics.hpp"

namespace commands
{
using namespace tap::algorithms::ballistics;
using communication::TurretData;
using modm::Vector3f;

void CommandSentryAim::initialize()
{
    lastTurretDataIndex = 0;
    testSeqIndex = 0;
    testSeqTicks = 0;
}

void CommandSentryAim::execute()
{
    // Boot test sequence: move through hardcoded poses before CV takes over.
    // Each pose is held for TICKS_PER_POSE ticks (~20 s at 500 Hz).
    if (testSeqIndex < NUM_TEST_POSES)
    {
        const TestPose& pose = TEST_POSES[testSeqIndex];
        turret->setTargetWorldAngles(turret->getChassisYaw() + pose.localYaw, pose.pitch);

        if (++testSeqTicks >= TICKS_PER_POSE)
        {
            testSeqTicks = 0;
            testSeqIndex++;
        }
        return;
    }

    // Guard 1: CV board must be communicating
    if (!drivers->cvBoard.isOnline()) return;

    // Guard 2: Only run when new CV data has arrived
    if (lastTurretDataIndex == drivers->cvBoard.turretDataIndex) return;
    lastTurretDataIndex = drivers->cvBoard.turretDataIndex;

    // Guard 3: Only run if CV is actively tracking a target
    TurretData data = drivers->cvBoard.getTurretData();
    if (!data.hasTarget) return;

    // Apply camera-to-barrel geometry offsets and swap axes
    // CV sends in camera frame (x=right, y=forward, z=up)
    // Turret frame: x=right, y=forward, z=up-from-barrel
    // y/z are swapped because pitch rotates in the y-z plane
    Vector3f targetPos(
        data.xPos + CAMERA_X_OFFSET,
        data.zPos + CAMERA_TO_PITCH,
        data.yPos + CAMERA_TO_BARRELS);
    Vector3f targetVel(data.xVel, data.zVel, data.yVel);
    Vector3f targetAcc(data.xAcc, data.zAcc, data.yAcc);

    if (USE_BALLISTICS)
    {
        // Rotate target vectors from turret-local to world frame
        // so that Z axis is consistently opposite gravity for the solver
        float a = turret->getCurrentLocalPitch();
        const float matData[9] = {1.0f, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a)};
        modm::Matrix3f rotMat(matData);
        targetPos = rotMat * targetPos;
        targetVel = rotMat * targetVel;
        targetAcc = rotMat * targetAcc;

        // Pack into kinematic state struct for the solver
        SecondOrderKinematicState kinState{targetPos, targetVel, targetAcc};

        float turretPitch = 0.0f;
        float turretYaw = 0.0f;
        float travelTime = 0.0f;

        // Run iterative ballistics solver
        // Returns: pitch angle (rad above level) and yaw delta (rad)
        // that will cause the projectile to hit the target
        bool validBallistics = findTargetProjectileIntersection(
            kinState,
            TARGET_PROJECTILE_VELOCITY,  // muzzle velocity m/s
            BALLISTIC_ITERATIONS,        // 2 iterations — enough for <5m
            &turretPitch,
            &turretYaw,
            &travelTime,
            -NOZZLE_TO_PITCH);

        if (validBallistics)
        {
            float currentWorldYaw = turret->getCurrentLocalYaw() + turret->getChassisYaw();
            turret->setTargetWorldAngles(currentWorldYaw + turretYaw, turretPitch);
        }
        // If not valid: target unreachable at this velocity — hold last angle
    }
}

void CommandSentryAim::end(bool) {}

bool CommandSentryAim::isFinished(void) const { return false; }
}  // namespace commands

///////////////////////////////////////////////
// # Only rebuilds changed files (~10 seconds)
// scons build robot=TARGET_SENTRY -j8
// scons run robot=TARGET_SENTRY
///////////////////////////////////////////////