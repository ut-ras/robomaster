#ifndef SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_

#include "modm/math/filter/pid.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

namespace subsystems
{
namespace shooter
{

// Agitator uses DJI motors
class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    AgitatorSubsystem(tap::Drivers *drivers, tap::motor::MotorId motorId, tap::can::CanBus canBusMotors);
    ~AgitatorSubsystem() = default;

    void initialize() override;

    void setMotorOutput(int desiredRPM);
    void rotateToTarget(int64_t targetPosition);
    bool isNearTarget(int64_t targetPosition);
    // Default refresh for Subsystem is no-op
    // void refresh() override;

private:
    const tap::motor::MotorId motorId;
    const tap::can::CanBus canBusMotors;
    uint16_t motorOutput; // Unused?
    modm::Pid<float> pidController; // Should the PID controller use int instead? Check kp
    modm::Pid<float> targetAnglePidController;
    tap::motor::DjiMotor motor;
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;
    static constexpr int NEAR_TARGET_THRESHOLD = 282; // 1/10 of one ball's worth of encoder ticks
};

}
}

#endif // SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
