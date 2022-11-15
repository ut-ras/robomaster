#include "tap/control/command.hpp"
#include "tap/drivers.hpp"

#include "chassis_subsystem.hpp"

using namespace tap::communication::serial;
using namespace modm;

namespace subsystems
{
namespace chassis
{
class SetKinematicsCommand : public tap::control::Command
{
public:
    SetKinematicsCommand(ChassisSubsystem *sub, tap::Drivers *drivers)
        : drivers(drivers),
          subsystem(sub),
          isRKeyPressed(false)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "set kinematics command"; }

private:
    static constexpr float DELTA_TIME = 0.002f;
    static constexpr float ANALOG_DEAD_ZONE = 0.2f;
    static constexpr float KEYBOARD_ACCEL = 6.0f;
    static constexpr float KEYBOARD_DECEL = 4.0f;

    tap::Drivers *drivers;
    ChassisSubsystem *subsystem;
    bool isRKeyPressed;
    Vector2f velocity = Vector2f(0.0f);
    float wZ = 0.0f;
};
}  // namespace chassis
}  // namespace subsystems