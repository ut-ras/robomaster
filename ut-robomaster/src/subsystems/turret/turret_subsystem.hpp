#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"

using namespace motor_controller;

namespace subsystems
{
namespace turret
{
class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(src::Drivers* drivers);
    void initialize() override;

    void setDesiredRpm(float desRpmYaw, float desRpmPitch);
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

private:
    static constexpr float PID_KP = 0.75f;
    static constexpr float PID_KI = 12.0f;
    static constexpr float PID_KD = 0.0f;

    float desiredRpmYaw;
    float desiredRpmPitch;

    MotorVelocityController yawMotor;
    MotorVelocityController pitchMotor;
};

}  // namespace turret
}  // namespace subsystems

#endif
