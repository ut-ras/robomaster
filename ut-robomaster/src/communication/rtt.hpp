#include <stdio.h>

#include "rtt/SEGGER_RTT.h"

int printf_rtt(const char* fmt, ...)
{
    int r;
    char data[64];
    va_list params;

    va_start(params, fmt);
    r = vsnprintf(data, 64, fmt, params);
    va_end(params);

    SEGGER_RTT_Write(0, data, r);
    return r;
}