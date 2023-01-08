#include "tap/control/command_scheduler.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/math/filter/pid.hpp"

#include "drivers.hpp"
#include "pid_motor_controller.hpp"

using namespace tap::motor;
using namespace pid_motor_controller;

namespace subsystems
{
namespace turret
{
class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(
        tap::Drivers* drivers,
        MotorId motorIdYaw = MOTOR6,
        MotorId motorIdPitch = MOTOR7);

    void initialize() override;

    void setDesiredRpm(float desRpmYaw, float desRpmPitch);
    void setDesiredRpm(float desRpm);
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

private:
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    static constexpr float PID_KP = 22.0f;
    static constexpr float PID_KI = 0.2f;
    static constexpr float PID_KD = 0.0f;
    static constexpr float PID_MAX_ERROR_SUM = 5000.0f;
    static constexpr float PID_MAX_OUTPUT = 16000.0f;

    modm::Pid<float> velocityPidYawMotor;
    modm::Pid<float> velocityPidPitchMotor;

    float desiredRpmYaw;
    float desiredRpmPitch;
    void updateMotorRpmPid(modm::Pid<float>* pid, DjiMotor* const motor, float desiredRpm);

    DjiMotor yawMotor;
    DjiMotor pitchMotor;
};

}  // namespace turret
}  // namespace subsystems