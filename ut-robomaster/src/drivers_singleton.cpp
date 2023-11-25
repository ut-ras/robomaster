#ifndef ENV_UNIT_TESTS

#include "drivers_singleton.hpp"

namespace src
{
/**
 * Class that allows one to construct a Drivers instance because of friendship
 * with the Drivers class.
 */
class DriversSingleton
{
public:
    static src::Drivers drivers;
};  // class DriversSingleton

src::Drivers DriversSingleton::drivers;

src::Drivers *DoNotUse_getDrivers() { return &DriversSingleton::drivers; }
}  // namespace src

#endif
