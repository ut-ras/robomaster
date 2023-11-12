#pragma once

const float IMU_KP = 0.1f;
const float IMU_KI = 0.0f;
const float IMU_SAMPLE_FREQUENCY = 500;

const uint32_t REFRESH_PERIOD = 2;              // ms
constexpr float DT = REFRESH_PERIOD / 1000.0f;  // refresh delta time (s)

#if defined(TARGET_STANDARD)
#include "robots/standard/standard_constants.hpp"

#elif defined(TARGET_HERO)
#include "robots/hero/hero_constants.hpp"

#elif defined(TARGET_SENTRY)
#include "robots/sentry/sentry_constants.hpp"

#endif