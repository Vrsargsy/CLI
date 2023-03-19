#include "server.h"

char *exec(char *cmd)
{
    FILE *fp;
    char *buffer = NULL;
    size_t buffer_size = 0;
    size_t buffer_capacity = 0;
    size_t bytes_read;

    fp = popen(cmd, "r");
	usleep(500);
    if (fp == NULL) {
        printf("Error: popen() failed:\n");
        return NULL;
    }

    while (1) {
        if (buffer_size + BUFFER_SIZE > buffer_capacity) {
            buffer_capacity = buffer_size + BUFFER_SIZE;
            buffer = realloc(buffer, buffer_capacity);
            if (buffer == NULL) {
                printf("Error: realloc() failed:\n");
                return NULL;
            }
        }

        bytes_read = fread(buffer + buffer_size, 1, BUFFER_SIZE, fp);
        if (bytes_read == 0) {
            break;
        }

        buffer_size += bytes_read;
    }

    buffer = realloc(buffer, buffer_size + 1);
    if (buffer == NULL) {
        printf("Error: realloc() failed: \n");
        return NULL;
    }

    buffer[buffer_size] = '\0';
	
    pclose(fp);
    return buffer;
}