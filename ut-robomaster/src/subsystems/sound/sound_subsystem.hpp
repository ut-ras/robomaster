#pragma once

#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems::sound
{
class SoundSubsystem : public tap::control::Subsystem
{
public:
    SoundSubsystem(src::Drivers* drivers);
    void initialize() override;
    void refresh() override;

    void setBuzzerFrequency(int frequency);
    void silence();

private:
    src::Drivers* drivers;
};
}  // namespace subsystems