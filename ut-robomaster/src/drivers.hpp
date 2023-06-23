/*
 * Copyright (c) 2021-2022 UT Robomaster
 *
 * This file is part of ut-robomaster.
 *
 * ut-robomaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ut-robomaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ut-robomaster.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef DRIVERS_HPP_
#define DRIVERS_HPP_

#include "tap/drivers.hpp"

#include "communication/beaglebone_communicator.hpp"
#include "robots/standard/robot_comms.hpp"
#include "utils/mouse_tracker.hpp"

namespace src
{
class Drivers : public tap::Drivers
{
    friend class DriversSingleton;

#ifdef ENV_UNIT_TESTS
public:
#endif
    Drivers() : tap::Drivers(), beagleboneCommunicator(this), terminal(this), mouseTracker(this) {}

public:
    communication::BeagleBoneCommunicator beagleboneCommunicator;
    comms::RobotComms terminal;
    mouse_tracker::MouseTracker mouseTracker;

    bool isKillSwitched()
    {
#ifdef TARGET_SENTRY
        return false;
#else
        return !remote.isConnected();
#endif
    }
};  // class Drivers

}  // namespace src

#endif  // DRIVERS_HPP_
