#pragma once
#include <cstdint>

struct Sound
{
    uint16_t data_length;
    uint16_t note_interval;
    uint8_t* data;
};