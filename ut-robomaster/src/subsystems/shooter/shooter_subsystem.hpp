#ifndef SHOOTER_SUBSYSTEM_HPP_
#define SHOOTER_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "agitator_subsystem.hpp"
#include "drivers.hpp"
#include "flywheel_subsystem.hpp"

namespace subsystems
{
namespace shooter
{

enum FiringState
{
    Idle,
    Ready,
    Firing
};

class ShooterSubsystem : public tap::control::Subsystem
{
public:
    ShooterSubsystem(src::Drivers* drivers);
    ~ShooterSubsystem() = default;

    void registerSubsystems();
    void initialize() override;
    void refresh() override;

    void setFiringState(FiringState state);

private:
    src::Drivers* drivers;
    FlywheelSubsystem flywheel;
    AgitatorSubsystem agitator;

    FiringState firingState = FiringState::Idle;
};

}  // namespace shooter
}  // namespace subsystems

#endif