
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char buf[64];

    int fd = open("/etc/hostname", O_RDONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    ssize_t n = read(fd, buf, sizeof buf);

    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    if (write(STDOUT_FILENO, buf, (size_t)n) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
