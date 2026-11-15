#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#define COUNT 1000

void *thread_function(void *arg)
{
    (void)arg;
    return NULL;
}

double get_time()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1000000000.0;
}

int main()
{
    double start, end;
    pid_t pid;
    pthread_t thread;

    start = get_time();

    for (int i = 0; i < COUNT; i++) {
        pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            _exit(0);
        }

        if (waitpid(pid, NULL, 0) < 0) {
            perror("waitpid");
            return 1;
        }
    }

    end = get_time();

    double process_time = end - start;

    start = get_time();

    for (int i = 0; i < COUNT; i++) {
        if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
            printf("Thread creation failed\n");
            return 1;
        }

        if (pthread_join(thread, NULL) != 0) {
            printf("Thread join failed\n");
            return 1;
        }
    }

    end = get_time();

    double thread_time = end - start;

    printf("1000 processes (fork + waitpid): %.6f seconds\n", process_time);
    printf("1000 threads (create + join):    %.6f seconds\n", thread_time);

    if (thread_time > 0) {
        printf("Process/thread ratio: %.2f times\n",
               process_time / thread_time);
    }

    return 0;
}
