#include "taproot/src/tap/algorithms/odometry/chassis_world_yaw_observer_interface.hpp"

#include "modm/math/geometry.hpp"
#include "robots/standard/robot_comms.hpp"
#include "drivers.hpp"
#include "modm/math/geometry/location_2d.hpp"

namespace subsystems::Odometry{
        
        class ChassisWorldYawOberver{
            public:
                void ChassisWorldYawOberver(src::Drivers* drivers){}
                const char* getName() override { return "ChassisWorldYawOberver";}
            private:
                src::Drivers* drivers;
        };

    };
}

