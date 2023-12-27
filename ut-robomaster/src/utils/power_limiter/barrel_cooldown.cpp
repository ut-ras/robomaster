#include "barrel_cooldown.hpp"

namespace power_limiter
{
float getRemainingCooldown(src::Drivers *drivers, BarrelId id)
{
    auto turret = drivers->refSerial.getRobotData().turret;

    float heat = 0.0f;
    float limit = 0.0f;

    switch (id)
    {
        case STANDARD1:
            heat = turret.heat17ID1;
            limit = turret.heatLimit17ID1;
            break;
        case STANDARD2:
            heat = turret.heat17ID2;
            limit = turret.heatLimit17ID2;
            break;
        case HERO:
            heat = turret.heat42;
            limit = turret.heatLimit42;
            break;
    }

    return limit - heat;
}
}  // namespace power_limiter