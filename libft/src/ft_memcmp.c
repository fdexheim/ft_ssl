#include "../inc/libft.h"

int					ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	uint8_t			*p1 = (uint8_t *)s1;
	uint8_t			*p2 = (uint8_t *)s2;

	i = 0;
	while (i < n)
	{
		if (p1[i] != p2[i])
			return (p1[i] - p2[i]);
		i++;
	}
	return (0);
}
