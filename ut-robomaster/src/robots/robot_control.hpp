#if defined(TARGET_STANDARD)
#include "robots/standard/standard_control.hpp"
using Control = StandardControl;

#elif defined(TARGET_HERO)
#include "robots/hero/hero_control.hpp"
using Control = HeroControl;

#elif defined(TARGET_SENTRY)
#include "robots/sentry/sentry_control.hpp"
using Control = SentryControl;

#endif