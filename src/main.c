#include "../inc/ft_ssl.h"

void				usage(void)
{
	printf("Usage: ./ft_ssl command [command opts] [command args]\n");
	return ;
}

void				close_env(t_ssl_env *env)
{
	free(env);
}

t_ssl_env			*setup_env(int argc, char **argv)
{
	t_ssl_env		*env;

	if ((env = malloc(sizeof(t_ssl_env))) == NULL)
	{
		printf("[ERROR] Failed memory allocation\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(env, sizeof(t_ssl_env));
	env->argc = argc;
	env->argv = argv;
	return (env);
}

int					main(int argc, char **argv)
{
	t_ssl_env		*env;

	if (argc <= 1)
	{
		usage();
		exit(EXIT_SUCCESS);
	}
	env = setup_env(argc, argv);
	parse_command(env);
//	run(env);
//	close_env();
}
