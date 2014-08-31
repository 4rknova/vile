#include "time.h"

#include <sys/time.h>

unsigned long get_ticks()
{
    struct timeval tv;
    if(gettimeofday(&tv, 0) != 0)
    return 0;
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
