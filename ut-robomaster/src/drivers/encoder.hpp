#pragma once

namespace driver
{
class Encoder
{
public:
    virtual ~Encoder() {}

    /// @brief Get current measured angle of the encoder
    /// @return Angle (revs)
    virtual float getAngle() = 0;
};
}  // namespace driver