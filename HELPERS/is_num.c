#include "helpers.h"

#include <ctype.h>

int	is_valid_port(char *str)
{
	// on different architectures, some ports is unaviable. in here i am checking, is it number ?
	for(size_t i = 0; str[i] != '\0'; i++)
	{
		if( isdigit(str[i]) == 0)
			return (0);
	}
	return (1);
}