#include "general.h"

static char	*substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	size_t			tlen;

	if (start > strlen(s))
		tlen = 0;
	else if (strlen(s) - start > len)
		tlen = len;
	else
		tlen = strlen(s) - start;
	str = (char *)malloc(tlen);
	if (str)
		strlcpy(str, s + start, tlen + 1);
	return (str);
}

static void	skipquotes1(char **promt, int *j)
{
	int	i;

	i = *j;
	if (i > 0 && (*promt)[i - 1] && (*promt)[i - 1] == 92)
	{
		(*j)++;
		return ;
	}
	if ((*promt)[i] && (*promt)[i] == 34)
	{
		i++;
		while ((*promt)[i] && (*promt)[i] != 34)
			i++;
		if ((*promt)[i])
			i++;
	}
	if ((*promt)[i] && (*promt)[i] == 39)
	{
		i++;
		while ((*promt)[i] && (*promt)[i] != 39)
			i++;
		if ((*promt)[i])
			i++;
	}
	*j = i;
}

static size_t	ft_count(char const *str, char j)
{
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] == j)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != j)
		{		
			if (str[i] == 34 || str[i] == 39)
				skipquotes1((char **)&str, (int *)&i);
			else
			{
				if (str[i])
				i++;
			}
		}
	}
	return (count);
}

static void	iterstring(char *s, int *len, char c)
{
	while (s[*len] != c && s[*len])
	{
		if (s[*len] == 34 || s[*len] == 39)
			skipquotes1(&s, len);
		else
			*len += 1;
	}
}

char	**split(char const *s, char c)
{
	if (!s || !*s)
		return NULL;
	char	**array;
	size_t	start;
	size_t	len;
	size_t	z;

	if (!s)
		return (0);
	array = (char **)malloc ((ft_count(s, c) + 1) * sizeof(char *));
	if (!array)
		return (0);
	z = 0;
	start = 0;
	len = 0;
	while (z < ft_count(s, c))
	{
		while (s[start] == c)
			start++;
		len = start;
		iterstring((char *)s, (int *)&len, c);
		array[z] = substr(s, start, (len - start));
		start = len;
		z++;
	}
	array[z] = 0;
	return (array);
}