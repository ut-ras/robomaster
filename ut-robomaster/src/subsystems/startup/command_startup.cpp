#include "command_startup.hpp"

namespace commands
{

void CommandStartup::initialize()
{
    int frequencies[11] = {110, 131, 220, 189, 310, 273, 114, 238, 331, 440, 0};

    for (int i = 0; i < 11; i++)
    {
        uint32_t time = tap::arch::clock::getTimeMilliseconds();
        uint32_t timeDiff = 0;
        startup->setBuzzerFrequency(frequencies[i]);
        while (timeDiff <= 1000)  // ms
        {
            timeDiff = tap::arch::clock::getTimeMilliseconds() - time;
        }
    }

    startup->silence();a
}

void CommandStartup::execute() {}

void CommandStartup::end(bool) {}

bool CommandStartup::isFinished() const { return false; }

}  // namespace commands