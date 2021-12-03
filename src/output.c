#include "../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void				display_data(void *hash, size_t size)
{
	unsigned char	*ptr = hash;

	for (uint32_t i = 0; i < size; i++)
	{
		if (ptr[i] <= 0xf)
			write(1, "0", 1);
		ft_put_size_t_hex(ptr[i]);
	}
}

//------------------------------------------------------------------------------
static void				display_after(char *src, bool string_mode)
{
	if (string_mode == true)
	{
		ft_putstr(" \"");
		ft_putstr(src);
		ft_putstr("\"");
	}
	else if (src != NULL)
	{
		ft_putstr(" ");
		ft_putstr(src);
	}
}

//------------------------------------------------------------------------------
static void				display_before(char *src, char *hash_name, bool string_mode)
{
	if (string_mode == true)
	{
		ft_putstr(hash_name);
		ft_putstr(" (\"");
		ft_putstr(src);
		ft_putstr("\") = ");
	}
	else if (src != NULL)
	{
		ft_putstr(hash_name);
		ft_putstr(" (");
		ft_putstr(src);
		ft_putstr(") = ");
	}
	else
	{
		ft_putstr("(stdin)= ");
	}
}

//------------------------------------------------------------------------------
void				display_hash(t_ssl_env *env, char *src, char *hash_name,
	t_ssl_data *output, bool string_mode)
{
	if (env->flags.q != true && env->flags.r == false)
	{
		display_before(src, hash_name, string_mode);
	}
	display_data(output->data, output->size);
	if (env->flags.q != true && env->flags.r == true)
	{
		display_after(src, string_mode);
	}
	ft_putstr("\n");
}
