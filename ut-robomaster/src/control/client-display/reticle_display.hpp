#include "tap/communication/referee/state_hud_indicator.hpp"
#include "tap/communication/serial/ref_serial.hpp"
#include "tap/control/command.hpp"
#include "modm/processing/resumable.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "client_display_subsystem.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using subsystems::control::ClientDisplaySubsystem;
using subsystems::turret::TurretSubsystem;

namespace commands
{
class reticle_display : public Command, modm::pt::Protothread
{
public:
    reticle_display(src::Drivers *drivers, TurretSubsystem *turret)
        : Command(),
          drivers(drivers),
          refSerialTransmitter(drivers),
          turret(turret)
    {
        addSubsystemRequirement(turret);
    }

    bool run();

    void initialize() override;
    void execute() override;
    void end(bool) override;
    bool isFinished() const override;
    const char *getName() const override { return "client display"; }

private:
    src::Drivers *drivers;
    TurretSubsystem *turret;
    RefSerialTransmitter refSerialTransmitter;
    const uint8_t graphicId[3] = {1, 0, 1};  // 3 byte identifier for this graphic element
    RefSerialData::Tx::Graphic1Message msg;

    static constexpr uint16_t CENTER_X = 1920 / 2;
    static constexpr uint16_t CENTER_Y = 1080 / 2;
    static constexpr uint16_t CIRCLE_SIZE = 10;

    // static int DROP_DISTANCE = 0;
    
    
};

}  