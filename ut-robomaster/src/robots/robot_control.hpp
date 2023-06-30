#ifndef ROBOT_CONTROL_HPP_
#define ROBOT_CONTROL_HPP_

#include "drivers.hpp"

namespace control {
    void initSubsystemCommands(src::Drivers *drivers);
}  // namespace control

#endif  // ROBOT_CONTROL_HPP_