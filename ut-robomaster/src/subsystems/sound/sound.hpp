#pragma once
#include <cstdint>

struct Sound
{
    uint16_t note_count;
    uint16_t note_interval;
    uint16_t* notes;
};