#include "helpers.h"

void	freeSplittedArr(char **mx)
{
	for (size_t i = 0; mx[i]; i++)
	{
		free(mx[i]);
		mx[i] = NULL;
	}
	 free(mx);
	 mx = NULL;
}