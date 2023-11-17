#pragma once

const float MAX_FLYWHEEL_SPEED = 200.0f;  // rps
const float MAX_FLYWHEEL_ACCEL = 50.0f;
const float MAX_AGITATOR_SPEED = 60.0f;

#if defined(TARGET_STANDARD)
#include "robots/standard/standard_constants.hpp"

#elif defined(TARGET_HERO)
#include "robots/hero/hero_constants.hpp"

#elif defined(TARGET_SENTRY)
#include "robots/sentry/sentry_constants.hpp"

#endif