#include "../inc/libft.h"

void		*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *dst_c = dest;
	const unsigned char *src_c = src;

	for (size_t i = 0; i < n; i++)
	{
		dst_c[i] = src_c[i];
	}
	return (dest);
}
