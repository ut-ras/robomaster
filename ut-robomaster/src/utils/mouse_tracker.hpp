#pragma once

#include "tap/drivers.hpp"

namespace mouse_tracker
{
class MouseTracker
{
public:
    MouseTracker(tap::Drivers* drivers) : drivers(drivers) {}

    void update()
    {
        x += drivers->remote.getMouseX();
        y += drivers->remote.getMouseY();
    }

    int32_t getX() { return x; }
    int32_t getY() { return y; }

    void reset()
    {
        x = 0;
        y = 0;
    }

private:
    tap::Drivers* drivers;
    int32_t x;
    int32_t y;
};
}  // namespace mouse_tracker