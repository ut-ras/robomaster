/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of Taproot.
 *
 * Taproot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Taproot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taproot.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "remote_map_state.hpp"

#include <numeric>

#include "tap/errors/create_errors.hpp"

namespace tap
{
namespace control
{
RemoteMapState::RemoteMapState(Remote::Switch swh, Remote::SwitchState switchState)
{
    if (swh == Remote::Switch::LEFT_SWITCH)
    {
        initLSwitch(switchState);
    }
    else
    {
        initRSwitch(switchState);
    }
}

RemoteMapState::RemoteMapState(Remote::SwitchState leftss, Remote::SwitchState rightss)
{
    initLSwitch(leftss);
    initRSwitch(rightss);
}

RemoteMapState::RemoteMapState(
    const std::list<Remote::Key> &keySet,
    const std::list<Remote::Key> &negKeySet)
{
    initKeys(keySet);
    initNegKeys(negKeySet);
}

RemoteMapState::RemoteMapState(
    RemoteMapState::MouseButton button,
    const std::list<Remote::Key> &keySet,
    const std::list<Remote::Key> &negKeySet)
{
    if (button == MouseButton::LEFT)
    {
        initLMouseButton();
    }
    else
    {
        initRMouseButton();
    }
    initKeys(keySet);
    initNegKeys(negKeySet);
}

RemoteMapState::RemoteMapState(MouseButton button)
{
    if (button == MouseButton::LEFT)
    {
        initLMouseButton();
    }
    else
    {
        initRMouseButton();
    }
}

void RemoteMapState::initLSwitch(Remote::SwitchState ss)
{
    if (ss == Remote::SwitchState::UNKNOWN)
    {
        return;
    }
    lSwitch = ss;
}

void RemoteMapState::initRSwitch(Remote::SwitchState ss)
{
    if (ss == Remote::SwitchState::UNKNOWN)
    {
        return;
    }
    rSwitch = ss;
}

void RemoteMapState::initKeys(uint16_t keys)
{
    if (keys == 0)
    {
        return;
    }
    if ((this->negKeys & keys) != 0)
    {
        return;
    }
    this->keys = keys;
}

void RemoteMapState::initNegKeys(uint16_t negKeys)
{
    if (negKeys == 0)
    {
        return;
    }
    if ((this->keys & negKeys) != 0)
    {
        return;
    }
    this->negKeys = negKeys;
}

void RemoteMapState::initKeys(const std::list<Remote::Key> &keySet)
{
    uint16_t keys = std::accumulate(
        keySet.begin(),
        keySet.end(),
        0,
        [](int acc, Remote::Key key) { return acc |= 1 << static_cast<uint16_t>(key); });
    initKeys(keys);
}

void RemoteMapState::initNegKeys(const std::list<Remote::Key> &negKeySet)
{
    // extract a bit form of the key set.
    uint16_t negKeys = std::accumulate(
        negKeySet.begin(),
        negKeySet.end(),
        0,
        [](int acc, Remote::Key key) { return acc |= 1 << static_cast<uint16_t>(key); });
    initNegKeys(negKeys);
}

void RemoteMapState::initLMouseButton() { lMouseButton = true; }

void RemoteMapState::initRMouseButton() { rMouseButton = true; }

bool RemoteMapState::stateSubsetOf(const RemoteMapState &other) const
{
    if (rSwitch != Remote::SwitchState::UNKNOWN && rSwitch != other.rSwitch)
    {
        return false;
    }
    if (lSwitch != Remote::SwitchState::UNKNOWN && lSwitch != other.lSwitch)
    {
        return false;
    }
    if ((keys & other.keys) != keys)
    {
        return false;
    }
    if (lMouseButton && other.lMouseButton != lMouseButton)
    {
        return false;
    }
    if (rMouseButton && other.rMouseButton != rMouseButton)
    {
        return false;
    }
    return true;
}

bool operator==(const RemoteMapState &rms1, const RemoteMapState &rms2)
{
    return rms1.lSwitch == rms2.lSwitch && rms1.rSwitch == rms2.rSwitch && rms1.keys == rms2.keys &&
           rms1.negKeys == rms2.negKeys && rms1.lMouseButton == rms2.lMouseButton &&
           rms1.rMouseButton == rms2.rMouseButton;
}

bool operator!=(const RemoteMapState &rms1, const RemoteMapState &rms2) { return !(rms1 == rms2); }

}  // namespace control
}  // namespace tap
