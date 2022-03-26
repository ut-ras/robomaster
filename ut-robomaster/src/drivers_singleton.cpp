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
