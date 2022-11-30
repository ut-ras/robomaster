#include "tap/control/command.hpp"
#include "tap/drivers.hpp"

#include "chassis_subsystem.hpp"

using namespace tap::communication::serial;
using namespace modm;

namespace subsystems
{
namespace chassis
{
class MoveChassisCommand : public tap::control::Command
{
public:
    MoveChassisCommand(ChassisSubsystem *sub, tap::Drivers *drivers)
        : drivers(drivers),
          subsystem(sub)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "set kinematics command"; }

private:
    void doControllerInput();
    void doKeyboardInput();

    static constexpr float DELTA_TIME = 0.002f;
    static constexpr float ANALOG_DEAD_ZONE = 0.1f;
    static constexpr float KEYBOARD_ACCEL = 6.0f;
    static constexpr float KEYBOARD_DECEL = 4.0f;

    tap::Drivers *drivers;
    ChassisSubsystem *subsystem;
    Vector2f inputMove = Vector2f(0.0f);
    float inputSpin = 0.0f;
    bool wasRKeyPressed = false;
    bool isBeyblading = false;
};
}  // namespace chassis
}  // namespace subsystems