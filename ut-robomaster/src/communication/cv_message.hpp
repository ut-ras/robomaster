#pragma once
#include <stdint.h>

namespace communication {

enum MessageTypes : uint8_t {
    CV_MESSAGE_TYPE_ODOMETRY_DATA = 1,
    CV_MESSAGE_TYPE_TURRET_AIM = 2,
    CV_MESSAGE_TYPE_COLOR_DATA = 3,
};

enum ColorTypes : uint8_t {
    COLOR_BLUE = 0,
    COLOR_RED = 1,
    COLOR_UNKNOWN = 2,
};

struct TurretData {
    float xPos;
    float yPos;
    float zPos;

    float xVel;
    float yVel;
    float zVel;

    float xAcc;
    float yAcc;
    float zAcc;

    bool hasTarget;
} modm_packed;

struct OdometryData {
    float xPos;
    float yPos;
    float zPos;

    float chassisPitch;
    float chassisYaw;
    float chassisRoll;

    float turretPitch;
    float turretYaw;
} modm_packed;

struct ColorData {
    ColorTypes color;
} modm_packed;

}   // namespace communication

 