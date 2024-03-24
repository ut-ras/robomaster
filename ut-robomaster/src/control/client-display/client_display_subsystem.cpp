#include "client_display_subsystem.hpp"

#include "tap/communication/sensors/buzzer/buzzer.hpp"

namespace subsystems::control
{
	ClientDisplaySubsystem::ClientDisplaySubsystem(
		tap::Drivers *drivers
		) : Subsystem(drivers)
	{
        tap::buzzer::playNote(&drivers->pwm, 440);
    }
}
