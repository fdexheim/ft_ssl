#include "../../inc/ft_ssl.h"

char			*md5_process_input(char *input)
{
	char		*ret;
	(void)input;

	if ((ret = malloc(64)) == NULL)
	{
		ft_putstr("[ERROR] malloc failure");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

void			md5_command(t_ssl_env *env, char **args)
{
	char		*input;
	char		*hashed;

	env->hash_size = 64;
	parse_md5(env, args);
	input = gather_full_input(env, 64);
	hashed = md5_process_input(input);

//	ft_putstr("input = ");
//	write(1, input, env->input_size);
//	ft_putstr("\nhash  = ");
//	write(1, hashed, env->hash_size);
//	write(1, "\n", 1);
	free(input);
	free(hashed);
}
