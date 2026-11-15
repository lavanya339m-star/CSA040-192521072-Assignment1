#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;
    char command[100];

    printf("--- zombie process demonstration ---\n");

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        _exit(0);
    }

    sleep(1);

    printf("Before wait(): zombie process\n");
    snprintf(command, sizeof(command),
             "ps -o pid,ppid,stat,comm -p %ld", (long)pid);
    system(command);

    waitpid(pid, &status, 0);

    printf("After wait(): zombie process removed\n");
    system(command);

    printf("\n--- orphan process demonstration ---\n");

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("orphan: parent before = %ld\n", (long)getppid());
        fflush(stdout);

        sleep(2);

        printf("orphan: parent after = %ld\n", (long)getppid());
        return 0;
    }

    printf("Parent process exiting.\n");
    return 0;
}
