#ifndef HELPERS_H
# define HELPERS_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

char	**split(char const *s, char c);
char	*itoa(int n);
void	freeSplittedArr(char **mx);
int		is_valid_port(char *str);

#endif