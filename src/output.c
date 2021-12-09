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
static void				display_after(t_ssl_env *env, char *src, e_ssl_output_mode mode)
{
	(void)env;
	if (mode == SSL_STRING_INPUT)
	{
		ft_putstr(" \"");
		ft_putstr(src);
		ft_putstr("\"");
	}
	else if (mode == SSL_FILE_INPUT)
	{
		ft_putstr(" ");
		ft_putstr(src);
	}
	else if (mode == SSL_STDIN_INPUT)
	{
	
	}
}

//------------------------------------------------------------------------------
static void				display_before(t_ssl_env *env, char *src, char *hash_name, e_ssl_output_mode mode)
{
	if (mode == SSL_STRING_INPUT)
	{
		ft_putstr(hash_name);
		ft_putstr(" (\"");
		ft_putstr(src);
		ft_putstr("\") = ");
	}
	else if (mode == SSL_FILE_INPUT)
	{
		ft_putstr(hash_name);
		ft_putstr(" (");
		ft_putstr(src);
		ft_putstr(") = ");
	}
	else if (mode == SSL_STDIN_INPUT)
	{
		if (env->flags.p == true)
		{
			ft_putstr("(\"");
			ft_putstr(src);
			ft_putstr("\")= ");
		}
		else
			ft_putstr("(stdin)= ");
	}
}

//------------------------------------------------------------------------------
void				display_hash(t_ssl_env *env, char *src, char *hash_name,
	t_ssl_data *output, e_ssl_output_mode mode)
{
	if (env->flags.q == false && (env->flags.r == false
		|| (mode == SSL_STDIN_INPUT && env->flags.p == true)))
	{
		display_before(env, src, hash_name, mode);
	}
	else if (env->flags.q == true && (mode == SSL_STDIN_INPUT && env->flags.p == true))
	{
		ft_putstr(src);
		ft_putstr("\n");
	}
	display_data(output->data, output->size);
	if (env->flags.q == false && env->flags.r == true)
	{
		display_after(env, src, mode);
	}
	ft_putstr("\n");
}
