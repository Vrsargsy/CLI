#include "SERVER/server.h"

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
	
    pclose(fp); // close the stream
    return buffer;
}




static size_t	num_size(long num)
{
	int	size;

	if (num < 0)
		size = 1;
	else
		size = 0;
	while (1)
	{
		num /= 10;
		size++;
		if (num == 0)
			break ;
	}
	return (size);
}

char	*itoa(int n)
{
	long	num;
	char	*arr;
	long	size;

	num = n;
	size = num_size(n);
	arr = malloc(size + 1);
	if (!arr)
		return (NULL);
	if (num < 0)
	{
		arr[0] = '-';
		num *= -1;
	}
	arr[size] = '\0';
	while (1)
	{
		arr[size - 1] = (num % 10) + '0';
		num /= 10;
		size--;
		if (num == 0)
			break ;
	}
	return (arr);
}