#include "command_startup.hpp"

namespace commands
{

void CommandStartup::initialize() { startTime = getTimeMilliseconds(); }

void CommandStartup::execute()
{
    uint32_t time = getTimeMilliseconds() - startTime;
    int note = static_cast<int>(time * timestep);

    if (note >= NOTE_COUNT)
    {
        startup->silence();
        done = true;
    }
    else if (note > lastNote)
    {
        lastNote = note;

        if (notes[note] == 0.0f)
        {
            startup->silence();
        }
        startup->setBuzzerFrequency(notes[note]);
    }
}

void CommandStartup::end(bool) {}
bool CommandStartup::isFinished() const { return done; }

}  // namespace commands