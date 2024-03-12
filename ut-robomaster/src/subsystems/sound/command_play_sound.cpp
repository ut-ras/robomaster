#include "command_play_sound.hpp"

namespace commands
{

void CommandPlaySound::initialize()
{
    timer.restart();
    subsystem->setBuzzerFrequency(sound.notes[0]);
}

void CommandPlaySound::execute()
{
    if (timer.execute() && ++note < sound.note_count)
    {
        uint16_t freq = sound.notes[note];

        if (freq != sound.notes[note - 1])  // only update if the note changed
        {
            if (freq > 0.0f)
            {
                subsystem->setBuzzerFrequency(freq);
            }
            else
            {
                subsystem->silence();  // silence also kills the duty cycle
            }
        }
    }
}

void CommandPlaySound::end(bool) { subsystem->silence(); }
bool CommandPlaySound::isFinished() const { return note >= sound.note_count; }

}  // namespace commands