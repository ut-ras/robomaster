#ifndef COMMAND_MOVE_CHASSIS_HPP_
#define COMMAND_MOVE_CHASSIS_HPP_

#include "tap/control/command.hpp"

#include "subsystems/turret/turret_subsystem.hpp"

#include "chassis_subsystem.hpp"
#include "drivers.hpp"

namespace subsystems
{
namespace chassis
{
using namespace tap::communication::serial;
using namespace modm;
using turret::TurretSubsystem;

class CommandMoveChassis : public tap::control::Command
{
public:
    CommandMoveChassis(src::Drivers *drivers, ChassisSubsystem *chassis, TurretSubsystem *turret)
        : drivers(drivers),
          chassis(chassis),
          turret(turret)
    {
        addSubsystemRequirement(chassis);
        // addSubsystemRequirement(turret); // this conflicts with the turret move command
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "move chassis command"; }

private:
    void doControllerInput();
    void doKeyboardInput();

    static constexpr float DELTA_TIME = 0.002f;
    static constexpr float ANALOG_DEAD_ZONE = 0.1f;
    static constexpr float KEYBOARD_ACCEL = 4.0f;
    static constexpr float KEYBOARD_DECEL = 3.0f;

    src::Drivers *drivers;
    ChassisSubsystem *chassis;
    TurretSubsystem *turret;

    Vector2f inputMove = Vector2f(0.0f);
    float inputSpin = 0.0f;
    bool wasRKeyPressed = false;
    bool isBeyblading = false;
};
}  // namespace chassis
}  // namespace subsystems

#endif
