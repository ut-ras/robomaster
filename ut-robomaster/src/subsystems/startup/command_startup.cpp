#include "command_startup.hpp"

namespace commands
{

void CommandStartup::initialize()
{
    timer.restart();
    startup->setBuzzerFrequency(NOTES[0]);
}

void CommandStartup::execute()
{
    if (timer.execute())
    {
        if (++note < NOTE_COUNT && NOTES[note] > 0.0f)
        {
            startup->setBuzzerFrequency(NOTES[note]);
        }
        else
        {
            startup->silence();
        }
    }
}

void CommandStartup::end(bool) { startup->silence(); }
bool CommandStartup::isFinished() const { return note >= NOTE_COUNT; }

}  // namespace commands