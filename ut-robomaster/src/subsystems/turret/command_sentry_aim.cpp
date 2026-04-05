#include "command_sentry_aim.hpp"

#include "tap/algorithms/ballistics.hpp"

namespace commands
{
using namespace tap::algorithms::ballistics;
using communication::TurretData;
using modm::Vector3f;

/**
 * @brief Initialize the sentry aim command state
 *
 * This method is called once when the command is first scheduled.
 * We reset the turret data index to ensure that when the command
 * becomes active, it immediately processes the next fresh CV packet
 * rather than waiting for a new index.
 *
 * @note This is critical for responsiveness - without resetting,
 * the command might skip the first CV packet after activation.
 */
void CommandSentryAim::initialize()
{
    // Reset index so we process fresh data on activation
    lastTurretDataIndex = 0;
}

/**
 * @brief Main execution loop for sentry autonomous aiming
 *
 * This method runs every 2ms control loop tick. It implements a
 * three-layer guard system to ensure safe and efficient operation:
 *
 * 1. CV communication guard - ensures serial link is active
 * 2. Data freshness guard - prevents reprocessing stale data
 * 3. Target detection guard - only aims when CV has a valid target
 *
 * When all guards pass, it applies full ballistic compensation to
 * account for projectile physics (gravity, velocity, travel time).
 *
 * @note This replaces the previous simple proportional aiming with
 * physics-based calculations that account for projectile drop.
 */
void CommandSentryAim::execute()
{
    // =================================================================
    // GUARD 1: CV BOARD COMMUNICATION CHECK
    // =================================================================
    // Why: The CV board provides target position data over serial.
    // If communication is lost, we cannot aim accurately and should
    // not attempt to move the turret (safety precaution).
    //
    // Importance: Prevents turret movement based on stale or invalid
    // data. The turret will hold its last commanded position safely.
    if (!drivers->cvBoard.isOnline()) return;

    // =================================================================
    // GUARD 2: FRESH DATA CHECK
    // =================================================================
    // Why: CV data arrives asynchronously (typically 30-60Hz) but
    // execute() runs at 500Hz (every 2ms). We must avoid reprocessing
    // the same target data hundreds of times.
    //
    // How: The CV board increments turretDataIndex with each new packet.
    // We compare against our cached lastTurretDataIndex to detect changes.
    //
    // Importance: Without this, we'd waste CPU cycles and potentially
    // cause erratic turret behavior from stale calculations.
    if (lastTurretDataIndex == drivers->cvBoard.turretDataIndex) return;
    lastTurretDataIndex = drivers->cvBoard.turretDataIndex;

    // =================================================================
    // GUARD 3: TARGET DETECTION CHECK
    // =================================================================
    // Why: CV may be online but not currently tracking any target.
    // We should only aim when there's an active target to track.
    //
    // How: Check the hasTarget flag from CV data packet.
    // This prevents unnecessary turret movement when scanning.
    TurretData data = drivers->cvBoard.getTurretData();
    if (!data.hasTarget) return;

    // =================================================================
    // COORDINATE SYSTEM TRANSFORMATION
    // =================================================================
    // Why: CV sends target data in camera coordinates, but we need
    // turret coordinates for aiming calculations.
    //
    // Camera frame: x=right, y=forward, z=up (standard camera coords)
    // Turret frame: x=right, y=forward, z=up-from-barrel (gravity direction)
    //
    // We apply geometric offsets to account for:
    // - CAMERA_X_OFFSET: camera not centered on turret axis
    // - CAMERA_TO_PITCH: distance from camera to pitch axis
    // - CAMERA_TO_BARRELS: distance from camera to barrel exit point
    //
    // Note: y/z axes are swapped because pitch rotation affects
    // the y-z plane in turret coordinates.
    Vector3f targetPos(
        data.xPos + CAMERA_X_OFFSET,     // Right-left offset
        data.zPos + CAMERA_TO_PITCH,     // Forward-back offset (swapped)
        data.yPos + CAMERA_TO_BARRELS);  // Up-down offset (swapped)
    Vector3f targetVel(data.xVel, data.zVel, data.yVel);
    Vector3f targetAcc(data.xAcc, data.zAcc, data.yAcc);

    // =================================================================
    // BALLISTICS SOLVER INTEGRATION
    // =================================================================
    // Why: Projectiles follow parabolic trajectories due to gravity.
    // Simple line-of-sight aiming will always miss at any significant
    // distance. We need physics-based calculations.
    //
    // How: The USE_BALLISTICS compile-time flag controls whether this
    // code is included. When true, we use iterative numerical methods
    // to solve the projectile intersection problem.
    if (USE_BALLISTICS)
    {
        // -----------------------------------------------------------------
        // WORLD FRAME ROTATION
        // -----------------------------------------------------------------
        // Why: The ballistics solver expects target vectors in world frame
        // where Z is consistently opposite gravity. However, CV gives us
        // turret-local coordinates that rotate with pitch.
        //
        // How: We rotate the target vectors using the current turret pitch
        // angle to transform from turret-local to world coordinates.
        // This ensures gravity always acts in -Z direction for the solver.
        //
        // Mathematical: Apply rotation matrix R(pitch) to transform
        // from turret frame to world frame before solving ballistics.
        float a = turret->getCurrentLocalPitch();
        const float matData[9] = {1.0f, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a)};
        modm::Matrix3f rotMat(matData);
        targetPos = rotMat * targetPos;
        targetVel = rotMat * targetVel;
        targetAcc = rotMat * targetAcc;

        // -----------------------------------------------------------------
        // KINEMATIC STATE PREPARATION
        // -----------------------------------------------------------------
        // Why: The solver needs target motion state (position, velocity,
        // acceleration) packaged in a specific struct format.
        //
        // How: Create SecondOrderKinematicState with the transformed
        // position, velocity, and acceleration vectors.
        SecondOrderKinematicState kinState{targetPos, targetVel, targetAcc};

        // -----------------------------------------------------------------
        // BALLISTICS SOLUTION VARIABLES
        // -----------------------------------------------------------------
        // These will be filled by the solver with the computed angles
        // and timing that will cause the projectile to hit the target.
        float turretPitch = 0.0f;  // Output: pitch angle above horizontal (radians)
        float turretYaw = 0.0f;    // Output: yaw angle delta from current (radians)
        float travelTime = 0.0f;   // Output: estimated flight time (seconds)

        // -----------------------------------------------------------------
        // ITERATIVE BALLISTICS SOLVER CALL
        // -----------------------------------------------------------------
        // Why: This is the core physics calculation. It iteratively solves
        // the nonlinear projectile intersection problem using numerical
        // methods (typically Newton-Raphson or similar).
        //
        // Parameters:
        // - kinState: target's position/velocity/acceleration in world frame
        // - TARGET_PROJECTILE_VELOCITY: muzzle speed (m/s) from constants
        // - BALLISTIC_ITERATIONS: solver precision (2 = good for <5m range)
        // - &turretPitch/&turretYaw: output angles in radians
        // - &travelTime: output flight time estimate
        // - -NOZZLE_TO_PITCH: barrel offset from pitch axis (negative=forward)
        //
        // Returns: true if solution found, false if target unreachable
        bool validBallistics = findTargetProjectileIntersection(
            kinState,
            TARGET_PROJECTILE_VELOCITY,  // muzzle velocity m/s
            BALLISTIC_ITERATIONS,        // 2 iterations — enough for <5m
            &turretPitch,
            &turretYaw,
            &travelTime,
            -NOZZLE_TO_PITCH);  // pitch axis offset (negative = forward)

        // -----------------------------------------------------------------
        // APPLY SOLVER RESULTS TO TURRET
        // -----------------------------------------------------------------
        // Why: If solver succeeded, command the turret to the calculated
        // angles. If failed, hold current position (safer than guessing).
        //
        // How: Convert from turret-local yaw delta to world yaw absolute,
        // then set both yaw and pitch angles simultaneously.
        if (validBallistics)
        {
            // Convert local yaw to world yaw and add solver's yaw delta
            // World yaw = current local yaw + chassis yaw + solver yaw delta
            float currentWorldYaw = turret->getCurrentLocalYaw() + turret->getChassisYaw();
            turret->setTargetWorldAngles(currentWorldYaw + turretYaw, turretPitch);
        }
        // If not valid: target unreachable at this velocity — hold last angle
        // This is safer than falling back to simple aiming, which would miss
    }
}

/**
 * @brief Command cleanup when ending
 *
 * Currently empty - the turret will hold its last commanded position.
 * Could add centering logic here if needed for shutdown behavior.
 */
void CommandSentryAim::end(bool) {}

/**
 * @brief Check if command should finish
 *
 * @return false - This command runs continuously as the turret's default.
 * It never finishes on its own - only interrupted by other commands.
 */
bool CommandSentryAim::isFinished() const { return false; }
}  // namespace commands
