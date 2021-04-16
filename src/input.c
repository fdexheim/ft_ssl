#include "../inc/ft_ssl.h"

//------------------------------------------------------------------------------
char			*gather_full_input_loop(t_ssl_env *env, int fd)
{
	char		buffer[FT_SSL_INPUT_BUFF_SIZE];
	char		*ret = NULL;
	ssize_t		read_size = 0;

	ft_bzero(buffer, FT_SSL_INPUT_BUFF_SIZE);
	while ((read_size = read(fd, buffer, FT_SSL_INPUT_BUFF_SIZE)) > 0)
	{
		if (env->allocated_size < env->input_size + read_size)
		{
			while (env->allocated_size < env->input_size + read_size)
				env->allocated_size += FT_SSL_INPUT_BUFF_SIZE;
			ret = bootleg_realloc(ret, env->input_size, env->allocated_size);
		}
		buffer_join(ret, buffer, env->input_size, read_size);
		env->input_size += read_size;
		ft_bzero(buffer, FT_SSL_INPUT_BUFF_SIZE);
	}
	return (ret);
}

//------------------------------------------------------------------------------
char			*gather_full_input(t_ssl_env *env, char *path)
{
	char		*ret;
	int			fd;

	ret = NULL;
	if (path == NULL)
	{
		ret = gather_full_input_loop(env, 0);
	}
	else
	{
		if ((fd = open(path, O_RDONLY)) < 0)
		{
			ft_putstr("[ERROR] Failed to open file '");
			ft_putstr(path);
			ft_putstr("'\n");
			return (NULL);
		}
		ret = gather_full_input_loop(env, fd);
		close(fd);
	}
	return (ret);
}

//------------------------------------------------------------------------------
void				env_soft_reset(t_ssl_env *env)
{
	env->input_size = 0;
	env->allocated_size = 0;
}
