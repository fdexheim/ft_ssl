#include "../../inc/ft_ssl.h"

void				write_base64(int fd, char *output, bool d, size_t output_size)
{
	size_t			iter = output_size / 64;

	for (size_t i = 0; i < iter; i++)
	{
		write(fd, output, 64);
		if (d == false)
			write(fd, "\n", 1);
		output += 64;
	}
	if (output_size % 64 > 0)
	{
		write(fd, output, output_size % 64);
		if (d == false)
			write(fd, "\n", 1);
	}
}

//------------------------------------------------------------------------------
void				display_base64(t_ssl_env *env, char *output, size_t output_size)
{
	int				fd;

	if (env->flags.o == true)
	{
		fd = open(env->flags.file_arg_out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
		{
			ft_putstr("Cannot access file output '");
			ft_putstr(env->flags.file_arg_out);
			ft_putstr("'\n");
			return ;
		}
		write_base64(fd, output, env->flags.d, output_size);
	}
	else
	{
		write_base64(1, output, env->flags.d, output_size);
	}
}
