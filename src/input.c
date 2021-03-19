#include "../inc/ft_ssl.h"

char			*buffer_join(char *input, char *add, size_t input_size,
	size_t section_size)
{
	printf(">>buffer_join called\n");
	char		*ret;

	ret = malloc(input_size + section_size);
	ft_bzero(ret, input_size + section_size);
	for (size_t i = 0; i < input_size; i++)
	{
		ret[i] = input[i];
	}
	for (size_t i = 0; i < (input_size + section_size); i++)
	{
		ret[input_size + i] = add[i];
	}
	printf(">>buffer_join exited\n");
	return (ret);
}

char			*gather_full_input_stdin(t_ssl_env *env, size_t section_size)
{
	printf(">>gather_full_input_stdin called\n");
	char		buffer[section_size];
	char		*ret;
	char		*tmp;
	ssize_t		read_size;
	size_t		iters;

	iters = 0;
	ft_bzero(buffer, section_size);
	while ((read_size = read(1, &buffer, section_size)) > 0)
	{
		tmp = ret;
		ret = buffer_join(ret, buffer, iters, section_size);
		if (iters > 0)
			free(tmp);
		ft_bzero(buffer, section_size);
		env->input_size += section_size;
		iters++;
	}
	write(1, "\n", 1);
	printf(">>gather_full_input_stdin exited\n");
	return (ret);
}

char			*gather_full_input(t_ssl_env *env, size_t section_size)
{
	char		*ret;

	ret = NULL;

	if (env->file_path == NULL)
	{
		ret = gather_full_input_stdin(env, section_size);
	}
	else
	{
		// do file case;
	}

	return (ret);
}
