#include "../../inc/ft_ssl.h"

char			*md5_process_input(char *input)
{
	char		*ret;

	ret = input;
	return (ret);
}

void			md5_command(t_ssl_env *env, char **args)
{
	char		*input;
	char		*hashed;

	env->hash_size = 512;
	hashed = malloc(env->hash_size);
	ft_bzero(hashed, env->hash_size);
	parse_md5(env, args);
	input = gather_full_input(env, 512);

	ft_putstr("input = \n");
	write(1, input, env->input_size);
	hashed = md5_process_input(input);
	ft_putstr("hash  = \n");
	write(1, hashed, env->hash_size);
	free(input);
}
