#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t event_received = 0;

void handler(int sig)
{
    (void)sig;
    event_received = 1;
}

double elapsed_ms(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

void sleep_ms(long milliseconds)
{
    struct timespec req;

    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000L;

    nanosleep(&req, NULL);
}

int main(void)
{
    struct timespec start, now;
    int checks = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &now);

        if (elapsed_ms(start, now) >= 55.0)
            break;

        sleep_ms(10);
        checks++;
    }

    clock_gettime(CLOCK_MONOTONIC, &now);

    printf("polling (10 ms interval): event at 55 ms, noticed after %.1f ms, %d checks\n",
           elapsed_ms(start, now), checks);

    signal(SIGUSR1, handler);
    event_received = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);

    sleep_ms(55);
    raise(SIGUSR1);

    clock_gettime(CLOCK_MONOTONIC, &now);

    if (event_received) {
        printf("interrupt style     : event at 55 ms, noticed after %.1f ms, 1 wake-up\n",
               elapsed_ms(start, now));
    }

    return 0;
}
