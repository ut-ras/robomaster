
#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems
{
class UTSubsystem : public tap::control::Subsystem
{
public:
    UTSubsystem(src::Drivers *drivers);

    ~UTSubsystem() = default;

    bool hardwareOk();
    bool isAmputated();
    bool setAmputated(bool newAmputatedValue);
protected:
    src::Drivers *drivers;
private:
    bool amputated = false;
};
}  // namespace subsystems
