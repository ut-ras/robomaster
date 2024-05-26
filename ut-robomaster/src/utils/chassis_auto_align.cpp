#include "chassis_auto_align.hpp"

#include <cmath>

float calculateAutoAlignCorrection(float yawAngle, float snapAngle)
{
    float scaledAngle = yawAngle / snapAngle;
    float correction = scaledAngle - roundf(scaledAngle);
    return (3.0f * correction - powf(correction, 3.0f)) / 2.0f;
}