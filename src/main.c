#include "../inc/ft_ssl.h"

void				list_commands(void)
{
	printf("\nStandard commands:\n");
	printf("\nMessage Digest commands:\n");
	printf("\nCipher commands:\n");
}

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

void				stdin_loop_mode(t_ssl_env *env)
{
	(void)env;
}

int					main(int argc, char **argv)
{
	t_ssl_env		*env;

	env = setup_env(argc, argv);
	if (argc == 1)
	{
		stdin_loop_mode(env);
	}
	else if (argc > 1)
	{
		parse_command(env, &argv[1]);
	}
//	run(env);
//	close_env();
}
