#pragma once

#include "tap/board/board.hpp"

namespace Board
{
using I2cSda = DigitalInPinPF0;
using I2cScl = DigitalInPinPF1;
using I2cMaster = I2cMaster2;

inline void initialize_i2c()
{
    I2cMaster::connect<I2cSda::Sda, I2cScl::Scl>();
    I2cMaster::initialize<Board::SystemClock>();
}
}  // namespace Board
