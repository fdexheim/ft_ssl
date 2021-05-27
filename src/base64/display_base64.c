#include "../../inc/ft_ssl.h"

void				write_base64(int fd, char *output, bool d)
{
	size_t			len;
	size_t			iter;

	len = ft_strlen(output);
	iter = len / 64;

	for (size_t i = 0; i < iter; i++)
	{
		write(fd, output, 64);
		if (d == false)
			write(fd, "\n", 1);
		output += 64;
	}
	if ((len = ft_strlen(output)) > 0)
	{
		write(fd, output, len);
		if (d == false)
			write(fd, "\n", 1);
	}
}

//------------------------------------------------------------------------------
void				display_base64(t_ssl_env *env, char *output)
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
		write_base64(fd, output, env->flags.d);
	}
	else
	{
		write_base64(1, output, env->flags.d);
	}
}
