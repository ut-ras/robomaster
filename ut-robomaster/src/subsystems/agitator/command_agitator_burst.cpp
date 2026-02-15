#include "command_agitator_burst.hpp"

using tap::communication::serial::RefSerialData;
namespace commands
{
void CommandAgitatorBurst::initialize() { initialPosition = agitator->getPosition(); }

void CommandAgitatorBurst::execute() { agitator->setBallsPerSecond(BALLS_PER_SEC); }

void CommandAgitatorBurst::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandAgitatorBurst::isFinished() const
{
    if (agitator->getPosition() - initialPosition >= numToFire / BALLS_PER_REV)
    {
        return true;
    }

    uint16_t barrel_heat_buffer = 0.0;
    if (drivers->refSerial.getRefSerialReceivingData())
    {
        tap::communication::serial::RefSerialData::Rx::GameType game_type =
            drivers->refSerial.getGameData().gameType;
        uint8_t robot_level = drivers->refSerial.getRobotData().robotLevel;
        if (game_type == RefSerialData::Rx::GameType::UNKNOWN)
        {
            drivers->rtt << "game type unknown";
            if (DEBUG_HEAT_BUFFER_ENABLED)
            {
                drivers->rtt << " overwriting game info to game_type: "
                             << (int)DEBUG_HEAT_BUFFER_GAME_TYPE
                             << " and robot_level: " << DEBUG_HEAT_BUFFER_ROBOT_LEVEL;
                game_type = DEBUG_HEAT_BUFFER_GAME_TYPE;
                robot_level = DEBUG_HEAT_BUFFER_ROBOT_LEVEL;
            }
            drivers->rtt.endl();
        }

        if (game_type ==
            tap::communication::serial::RefSerialData::Rx::GameType::ROBOMASTER_RMUL_1V1)
        {
            barrel_heat_buffer = BARREL_HEAT_BUFFER_1V1;
        }
        else if (
            game_type ==
            tap::communication::serial::RefSerialData::Rx::GameType::ROBOMASTER_RMUL_3V3)
        {
            if (robot_level <= 0 || robot_level > 10)
            {
                drivers->rtt << "ERROR UNRECOGNIZED ROBOT LEVEL: " << robot_level
                             << ". DEFAULTING TO " << barrel_heat_buffer << " HEAT BUFFER";
                drivers->rtt.endl();
            }
            else
            {
                barrel_heat_buffer = BARREL_HEAT_BUFFER_3V3[robot_level - 1];
            }
        }
    }

    if (drivers->refSerial.getRefSerialReceivingData() &&
        power_limiter::getRemainingHeat(drivers, barrelId) <= barrel_heat_buffer)
    {
        return true;
    }

    return false;
}
}  // namespace commands