#include "command_play_sound.hpp"

namespace commands
{

void CommandPlaySound::initialize()
{
    timer.restart(0);
    read_index = 0;
}

void CommandPlaySound::execute()
{
    if (read_index >= sound.data_length) return;

    if (timer.execute())
    {
        timer.restart(sound.note_interval);

        uint8_t data = sound.data[read_index];
        bool is_repeating = data > 127;

        if (is_repeating)
        {
            playNote(data - (1 << 7));
            int reps = sound.data[read_index + 1];
            timer.restart(sound.note_interval * (reps + 1));
            read_index += 2;
        }
        else
        {
            playNote(data);
            timer.restart(sound.note_interval);
            read_index++;
        }
    }
}

void CommandPlaySound::end(bool) { subsystem->silence(); }
bool CommandPlaySound::isFinished() const
{
    return read_index >= sound.data_length && timer.isExpired();
}

void CommandPlaySound::playNote(uint8_t note)
{
    if (note > 0.0f)
    {
        // +1 octave adjustment for onlinesequencer.net
        uint32_t freq = 440.0f * powf(2.0f, (note - 69.0f) / 12.0f + 1.0f);
        subsystem->setBuzzerFrequency(freq);
    }
    else
    {
        subsystem->silence();  // silence also kills the duty cycle
    }
}

}  // namespace commands