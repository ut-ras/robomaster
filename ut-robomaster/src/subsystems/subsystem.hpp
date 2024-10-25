#pragma once
#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems
{
class UTSubsystem : public tap::control::Subsystem
{
public:
    UTSubsystem(src::Drivers *drivers);

    ~UTSubsystem() = default;

    virtual bool hardwareOk();
    bool isAmputated();
    void setAmputated(bool newAmputatedValue);

protected:
    src::Drivers *drivers;

private:
    bool amputated = false;
};
}  // namespace subsystems
