#pragma once

#if defined(TARGET_STANDARD)
#include "robots/standard/standard_constants.hpp"

#elif defined(TARGET_HERO)
#include "robots/hero/hero_constants.hpp"

#elif definted(TARGET_SENTRY)
#include "robots/sentry/sentry_constants.hpp"

#endif