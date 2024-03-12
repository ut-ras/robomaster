#include "command_play_sound.hpp"

namespace commands
{

void CommandPlaySound::initialize()
{
    timer.restart();
    sound->setBuzzerFrequency(NOTES[0]);
}

void CommandPlaySound::execute()
{
    if (timer.execute() && ++note < NOTE_COUNT)
    {
        uint16_t freq = NOTES[note];

        if (freq != NOTES[note - 1])  // only update if the note changed
        {
            if (freq > 0.0f)
            {
                sound->setBuzzerFrequency(freq);
            }
            else
            {
                sound->silence();  // silence also kills the duty cycle
            }
        }
    }
}

void CommandPlaySound::end(bool) { sound->silence(); }
bool CommandPlaySound::isFinished() const { return note >= NOTE_COUNT; }

}  // namespace commands