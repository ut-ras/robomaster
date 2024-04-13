#ifndef CHASSIS_SUBSYSTEM_HPP_
#define CHASSIS_SUBSYSTEM_HPP_

#include "tap/architecture/periodic_timer.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "robots/robot_constants.hpp"
#include "utils/motor_controller/motor_controller.hpp"
#include "utils/power_limiter/power_limiter.hpp"

#include "drivers.hpp"

using namespace tap::communication::sensors::imu;
using namespace modm;
using namespace motor_controller;

namespace subsystems
{
namespace chassis
{
class ChassisSubsystem : public tap::control::Subsystem
{
public:
    ChassisSubsystem(src::Drivers* drivers);

    void initialize() override;

    void refresh() override;

    void limitChassisPower();

    void runHardwareTests() override;

    /// @brief Update robot motion based on simple input controls. Inputs are scaled and corrected
    /// to avoid over-driving motors. This logic can be adjusted to create various input schemes.
    /// @param move Linear movement (magnitude should be within [0,1])
    /// @param spin Angular rotation (value should be within [-1,1])
    void input(Vector2f move, float spin);

    /// @brief Reconstruct current velocities based on measured wheel motion.
    /// @return x,y is linear velocity (m/s) and z is angular velocity (rad/s)
    Vector3f measureVelocity();

    const char* getName() override { return "Chassis subsystem"; }

private:
    src::Drivers* drivers;
    power_limiter::PowerLimiter powerLimiter;
    MotorVelocityController wheels[WHEELS];
    float targetWheelVels[WHEELS] = {0.0f, 0.0f, 0.0f, 0.0f};
    tap::arch::PeriodicMilliTimer encoderTimer;

    /// @brief Calculate and set wheel velocities for desired robot motion (based on
    /// https://research.ijcaonline.org/volume113/number3/pxc3901586.pdf).
    /// @param v Linear velocity (m/s)
    /// @param wZ Angular velocity (rad/s)
    void setMecanumWheelVelocities(Vector2f v, float wZ);

    static constexpr float ENERGY_BUFFER_LIMIT_THRESHOLD = 60.0f;
#if defined(TARGET_STANDARD)
    static constexpr float ENERGY_BUFFER_CRIT_THRESHOLD = 10.0f;
#elif defined(TARGET_HERO)
    static constexpr float ENERGY_BUFFER_CRIT_THRESHOLD = 30.0f;
#elif defined(TARGET_SENTRY)
    static constexpr float ENERGY_BUFFER_CRIT_THRESHOLD = 30.0f;
#endif
};
}  // namespace chassis
}  // namespace subsystems

#endif
