#include "helpers.h"

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