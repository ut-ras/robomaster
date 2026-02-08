#include "command_agitator_continuous.hpp"

using tap::communication::serial::RefSerialData;
namespace commands
{
using tap::communication::serial::Remote;

void CommandAgitatorContinuous::initialize()
{
    jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
    isJammed = false;
}

void CommandAgitatorContinuous::execute()
{
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

    if (drivers->refSerial.getRefSerialReceivingData())
    {
        drivers->rtt << "barrel heat: " << power_limiter::getRemainingHeat(drivers, barrelId)
                     << "; heat buffer: " << barrel_heat_buffer;
        if (power_limiter::getRemainingHeat(drivers, barrelId) < barrel_heat_buffer)
        {
            drivers->rtt << " OVERHEATED";
        }
        drivers->rtt.endl();
    }
    // UNCOMMENT BARREL OVERHEAT ONCE WE FIX IT LMAO
    if (isJammed)
    {
        if (unjammingTimeout.isExpired())
        {
            jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
            isJammed = false;
        }

        agitator->setBallsPerSecond(-UNJAM_SPEED);
    }
    else if (  // barrel overheat
        drivers->refSerial.getRefSerialReceivingData() &&
        power_limiter::getRemainingHeat(drivers, barrelId) < barrel_heat_buffer)
    {
        jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
        agitator->setBallsPerSecond(0.0f);
    }
    else  // heat remaining and no jams
    {
        float speedRatio = abs(agitator->getVelocity()) / (BALLS_PER_SEC / BALLS_PER_REV);

        if (speedRatio > JAM_TRIGGER_RATIO)  // no jam
        {
            jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
        }
        else if (jamTriggerTimeout.isExpired())  // jam detected
        {
            unjammingTimeout.restart(UNJAM_DURATION * 1000);
            isJammed = true;
        }

        agitator->setBallsPerSecond(BALLS_PER_SEC);
    }
    // agitator->setBallsPerSecond(BALLS_PER_SEC);
}

void CommandAgitatorContinuous::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandAgitatorContinuous::isFinished() const
{
    if (checkMode &&
        drivers->remote.getSwitch(Remote::Switch::RIGHT_SWITCH) == Remote::SwitchState::DOWN)
    {
        return true;
    }
    return false;
}
}  // namespace commands