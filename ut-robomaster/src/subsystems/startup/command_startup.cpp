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
    if (timer.execute() && ++note < NOTE_COUNT)
    {
        uint16_t freq = NOTES[note];

        if (freq != NOTES[note - 1])  // only update if the note changed
        {
            if (freq > 0.0f)
            {
                startup->setBuzzerFrequency(freq);
            }
            else
            {
                startup->silence();  // silence also kills the duty cycle
            }
        }
    }
}

void CommandStartup::end(bool) { startup->silence(); }
bool CommandStartup::isFinished() const { return note >= NOTE_COUNT; }

}  // namespace commands