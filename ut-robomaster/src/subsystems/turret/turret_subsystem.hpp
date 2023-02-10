#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"

using namespace motor_controller;
using modm::Vector3f;

namespace subsystems
{
namespace turret
{
enum AimStrategy
{
    Manual,
    AutoAim,
    AimAssist
};

class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(src::Drivers* drivers);
    void initialize() override;

    void setDesiredAngles(float yaw, float pitch);

    /// @brief Input target data from CV (relative to camera)
    void inputTargetData(Vector3f position, Vector3f velocity, Vector3f acceleration);

    void setAimStrategy(AimStrategy aimStrategy);
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

private:
    static constexpr float PID_KP = 0.1f;
    static constexpr float PID_KI = 0.1f;
    static constexpr float PID_KD = 0.0f;

    float desiredYaw;
    float desiredPitch;

    Vector3f targetPosition = Vector3f(0.0f);
    Vector3f targetVelocity = Vector3f(0.0f);
    Vector3f targetAcceleration = Vector3f(0.0f);

    AimStrategy aimStrategy = AimStrategy::Manual;

    MotorPositionController yawMotor;
    MotorPositionController pitchMotor;
};

}  // namespace turret
}  // namespace subsystems

#endif
