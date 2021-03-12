#include "../inc/ft_ssl.h"

void			sha256_placeholder(t_ssl_env *env)
{
	(void)env;
	printf("sha_placeholder called\n");
}

void			md5_placeholder(t_ssl_env *env)
{
	(void)env;
	printf("md5_placeholder called\n");
}

void			parse_flag_arg(t_ssl_env *env, char *arg)
{
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
		env->flags.p = arg[i] == 'p' ? true : env->flags.p;
		env->flags.q = arg[i] == 'q' ? true : env->flags.q;
		env->flags.r = arg[i] == 'r' ? true : env->flags.r;
		env->flags.s = arg[i] == 's' ? true : env->flags.s;
	}
}

void			parse_flags(t_ssl_env *env)
{
	for (int i = 1; i < env->argc; i++)
	{
		if (env->argv[i][0] == '-')
			parse_flag_arg(env, env->argv[i]);
	}
}

void			parse_command(t_ssl_env *env)
{
	t_ssl_command		cmds[] = {
		{ "md5", md5_placeholder },
		{ "sha256", sha256_placeholder },
		{ NULL, NULL }
	};
	uint32_t			index = 0;

	parse_flags(env);
	while (cmds[index].command_name != NULL)
	{
		if (!ft_strcmp(env->argv[1], cmds[index].command_name))
		{
			cmds[index].f(env);
			return ;
		}
		index++;
	}
	printf("%s: Error: '%s' is an invalid command\n", env->argv[0],
		env->argv[1]);
}
