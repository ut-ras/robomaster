#include "tap/drivers.hpp"

#include "client_display_subsystem.hpp"

namespace commands
{
class ClientDisplayCommand
{
    ClientDisplayCommand::ClientDisplayCommand(tap::Drivers &drivers)
        : Command()

              bool ClientDisplayCommand::run()
    {
        PT_BEGIN();

        PT_WAIT_UNTIL(drivers.refSerial.getRefSerialReceivingData());

        PT_CALL(TestIndicator::sendInitialGraphics());

        PT_END();
    }
}
}  // namespace commands
