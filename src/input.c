#include "../inc/ft_ssl.h"

void			dump_buffer(void *buff, size_t size)
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

char			*buffer_join(char *input, char *add, size_t input_size,
	size_t add_size)
{
	char		*ret;
	size_t		new_size = input_size + add_size;

	if ((ret = malloc(new_size)) == NULL)
	{
		ft_putstr("[ERROR] malloc error\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(ret, input_size + add_size);
	for (size_t i = 0; i < input_size; i++)
	{
		ret[i] = input[i];
	}
	for (size_t i = 0; i < add_size; i++)
	{
		ret[input_size + i] = add[i];
	}
	return (ret);
}

char			*gather_full_input_stdin(t_ssl_env *env, size_t section_size, int fd)
{
	char		buffer[section_size];
	char		*ret;
	char		*tmp;
	ssize_t		read_size;
	size_t		iters = 0;

	ft_bzero(buffer, section_size);
	while ((read_size = read(fd, &buffer, section_size)) > 0)
	{
		tmp = ret;
		ret = buffer_join(ret, buffer, env->input_size, read_size);
		if (iters > 0)
		{
			free(tmp);
			env->input_size += read_size;
		}
		else
			env->input_size = read_size;
		ft_bzero(buffer, section_size);
		iters++;
	}
	write(1, "\n", 1);
//	dump_buffer(ret, env->input_size);
	return (ret);
}

char			*pad_buffer(t_ssl_env *env, char *src, size_t src_size, size_t section_size)
{
	size_t		padded_size = 0;
	char		*ret;

	while (padded_size < src_size || padded_size < section_size)
	{
		padded_size += section_size;
	}
	ret = (char *)malloc(padded_size);
	ft_bzero(ret, padded_size);
	for (size_t i = 0; i < src_size; i++)
	{
		ret[i] = src[i];
	}
	env->input_size = padded_size;
	free(src);
	return (ret);
}

char			*gather_full_input(t_ssl_env *env, size_t section_size)
{
	char		*ret;
	int			fd;

	ret = NULL;
	if (env->file_path == NULL)
	{
		ret = gather_full_input_stdin(env, section_size, 0);
	}
	else
	{
		if ((fd = open(env->file_path, O_RDONLY)) < 0)
		{
			ft_putstr("[ERROR] Failed to open file '");
			ft_putstr(env->file_path);
			ft_putstr("'\n");
			exit(EXIT_FAILURE);
		}
		ret = gather_full_input_stdin(env, section_size, fd);
		close(fd);
	}
	printf("Final input size (before padding) = %ld\n", env->input_size);
	ret = pad_buffer(env, ret, env->input_size, section_size);
	printf("Final input size (after  padding) = %ld\n", env->input_size);

	return (ret);
}
