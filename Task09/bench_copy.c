#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: %s <rw|stdio|mmap> <input> <output> <buffer_size>\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *input = argv[2];
    char *output = argv[3];
    size_t buffer_size = (size_t)atol(argv[4]);

    if (buffer_size == 0) {
        printf("Invalid buffer size\n");
        return 1;
    }

    if (strcmp(mode, "rw") == 0) {
        int in = open(input, O_RDONLY);
        int out = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        char *buffer = malloc(buffer_size);
        ssize_t n;

        if (in < 0 || out < 0 || buffer == NULL) {
            perror("File operation");
            return 1;
        }

        while ((n = read(in, buffer, buffer_size)) > 0) {
            if (write(out, buffer, n) != n) {
                perror("write");
                return 1;
            }
        }

        free(buffer);
        close(in);
        close(out);
    }
    else if (strcmp(mode, "stdio") == 0) {
        FILE *in = fopen(input, "rb");
        FILE *out = fopen(output, "wb");
        char *buffer = malloc(buffer_size);
        size_t n;

        if (!in || !out || !buffer) {
            perror("File operation");
            return 1;
        }

        while ((n = fread(buffer, 1, buffer_size, in)) > 0) {
            if (fwrite(buffer, 1, n, out) != n) {
                perror("fwrite");
                return 1;
            }
        }

        free(buffer);
        fclose(in);
        fclose(out);
    }
    else if (strcmp(mode, "mmap") == 0) {
        int in = open(input, O_RDONLY);
        struct stat st;

        if (in < 0 || fstat(in, &st) < 0) {
            perror("Input file");
            return 1;
        }

        size_t size = st.st_size;

        if (size > 0) {
            char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, in, 0);
            int out = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (data == MAP_FAILED || out < 0) {
                perror("mmap or output file");
                return 1;
            }

            if (write(out, data, size) != (ssize_t)size) {
                perror("write");
                return 1;
            }

            munmap(data, size);
            close(out);
        }

        close(in);
    }
    else {
        printf("Unknown mode\n");
        return 1;
    }

    printf("Copy completed using %s mode with buffer size %zu bytes\n",
           mode, buffer_size);

    return 0;
}
