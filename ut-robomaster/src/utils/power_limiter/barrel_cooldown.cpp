#include "barrel_cooldown.hpp"

namespace power_limiter
{
float getRemainingHeat(src::Drivers *drivers, BarrelId id)
{
    auto turret = drivers->refSerial.getRobotData().turret;

    float heat = 0.0f;

    switch (id)
    {
        case STANDARD1:
            heat = turret.heat17ID1;
            break;
        case STANDARD2:
            heat = turret.heat17ID2;
            break;
        case HERO:
            heat = turret.heat42;
            break;
    }

    return turret.heatLimit - heat;
}
}  // namespace power_limiter