#include "../inc/ft_ssl.h"

//------------------------------------------------------------------------------
void				dump_buffer(void *buff, size_t size)
{
	unsigned char	*tmp = buff;

	for (size_t i = 0; i < size; i++)
	{
		if (i % 16 == 0 && i > 0)
			write(1, "\n", 1);
		if (tmp[i] <= 0xf)
			write(1, "0", 1);
		ft_put_size_t_hex(tmp[i]);
		write(1, " ", 1);
	}
	write(1, "\n", 1);
}

//------------------------------------------------------------------------------
void				buffer_join(void *input, void *add, size_t input_size,
	size_t add_size)
{
	char			*input_ptr = input;
	char			*add_ptr = add;

	for (size_t i = 0; i < add_size; i++)
	{
		input_ptr[input_size + i] = add_ptr[i];
	}
}

//------------------------------------------------------------------------------
void				*memdup(void *src, size_t size)
{
	unsigned char	*ret;

	if ((ret = malloc(size)) == NULL)
	{
		ft_putstr("Bad malloc in memdup\n");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(ret, src, size);
	return (ret);
}

//------------------------------------------------------------------------------
void				*bootleg_realloc(void *src, size_t old_size, size_t new_size)
{
	char			*ret;
	char			*ptr;

	ptr = src;
	if (old_size > new_size)
		return (src);
	ret = malloc(new_size);
	if (ret == NULL)
	{
		ft_putstr("Bad memory reallocation\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(ret, new_size);
	if (src == NULL)
		return (ret);
	for (size_t i = 0; i < old_size; i++)
	{
		ret[i] = ptr[i];
	}
	if (src != NULL)
		free(src);
	return (ret);
}
