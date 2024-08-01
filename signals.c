#include "main.h"
#include "parsing.h"

t_feedback feedback(int status)
{
    t_feedback feed;

    // the status contains:
    // 1. the signal --> the first 8 bits
    // 2. the status --> the last 8 bits

    // shift the status to the right by 8 bits
    feed.signal = status >> 8;
    // mask the status with 255
    feed.status = status & 255;

    return (feed);
}

// this function is used to send a signal to a process
void send(int signal, pid_t pid)
{
    kill(pid, signal);
}