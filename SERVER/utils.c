#include "server.h"

/* char *exec(char *command)
{

//    char buffer[BUFFER_SIZE + 1] = {0};
   char *result = malloc(100000 + 1);
   unsigned int  i = 0;
//    unsigned int  ri = 0;
   
   result[100000] = '\0';
   bzero(result, 100000);

   FILE* pipe = popen(command, "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
    while ( (result[i] = fgetc(pipe)) != EOF)
    {
        i++;
    }
    result[i] = '\0';
    puts(result);
    pclose(pipe);
    return result;
} */

char *exec(char *cmd)
{
    char    *result = NULL;
    char ch;
    ssize_t len = 0;
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        return "Failed to execute\n";
    
    while((fgetc(pipe)) != EOF)
        len++;
    
    result = (char *)malloc(len + 1);

    bzero(result, len + 1);
    result[len + 1] = '\0';
    
    printf(RED"Output length: %lu\n"RESET, len);
    len = 0;
    while((ch = fgetc(pipe)) != EOF)
        result[len] = ch;
    pclose(pipe);
    return result;
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