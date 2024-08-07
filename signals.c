#include "main.h"
#include "parsing.h"

t_feedback *feedback(t_db *db, int status)
{
    t_feedback *feed;


    feed = gc_malloc(db, sizeof(t_feedback));
    if (!feed)
        return (NULL);
    // the status contains:
    // 1. the signal --> the first 8 bits
    // 2. the status --> the last 8 bits

    // shift the status to the right by 8 bits
    feed->signal = status >> 8;
    // mask the status with 255
    feed->status = status & 255;

    return (feed);
}

