#pragma once
#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems
{
class Subsystem : public tap::control::Subsystem
{
public:
    Subsystem(src::Drivers *drivers);

    ~Subsystem() = default;

    virtual bool hardwareOk();
    bool isAmputated();
    void setAmputated(bool newAmputatedValue);

protected:
    src::Drivers *drivers;

private:
    bool amputated = false;
};
}  // namespace subsystems
