#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "test_subsystem.hpp"

using subsystems::test::TestSubsystem;
using tap::communication::serial::Remote;
using tap::control::Subsystem;

namespace subsystems
{
namespace test
{
class TestCommand : public tap::control::Command
{
public:
    TestCommand(TestSubsystem* sub, tap::Drivers* drivers) : drivers(drivers), subsystem(sub)
    {
        addSubsystemRequirement(sub);
    }

    /**
     * The initial subroutine of a command.  Called once when the command is
     * initially scheduled.
     */
    void initialize() override;

    /**
     * The main body of a command.  Called repeatedly while the command is
     * scheduled.
     */
    void execute() override;

    /**
     * The action to take when the command ends.  Called when either the command
     * finishes normally, or when it interrupted/canceled.
     *
     * @param interrupted whether the command was interrupted/canceled
     */
    void end(bool interrupted) override;

    /**
     * Whether the command has finished.  Once a command finishes, the scheduler
     * will call its end() method and un-schedule it.
     *
     * @return whether the command has finished.
     */
    bool isFinished() const override;

    const char* getName() const override { return "example"; }

private:
    tap::Drivers* drivers;
    TestSubsystem* subsystem;
};
}  // namespace test
}  // namespace subsystems