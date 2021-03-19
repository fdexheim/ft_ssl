#include "../../inc/ft_ssl.h"

void						parse_noargs_flag(t_ssl_env *env, char *arg)
{
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
		env->flags.p = arg[i] == 'p' ? true : env->flags.p;
		env->flags.q = arg[i] == 'q' ? true : env->flags.q;
		env->flags.r = arg[i] == 'r' ? true : env->flags.r;
		env->flags.s = arg[i] == 's' ? true : env->flags.s;
	}
}

void						placeholder_handle_flag_a(t_ssl_env *env, char *arg)
{
	(void)env;
	printf("placeholder_handle_flag_a called with arg %s\n", arg);
}

void						placeholder_handle_flag_b(t_ssl_env *env, char *arg)
{
	(void)env;
	printf("placeholder_handle_flag_b called with arg %s\n", arg);
}

bool						check_nb_args_required(char **args, uint32_t nb_arg)
{
	for (uint32_t i = 1; i <= nb_arg; i++)
	{
		if (args[i] == NULL)
				printf("Missing arguments for flag %s\n", args[0]);
			return (false);
	}
	return (true);
}

uint32_t					check_arg_flags(t_ssl_env *env, char **args)
{
	const t_ssl_arg_flags	ssl_arg_flags[] = {
		{ "-a", 1, placeholder_handle_flag_a },
		{ "-b", 1, placeholder_handle_flag_b },
		{ NULL, 0, NULL }
	};

//	printf("check_arg_flags called, arg being compared == '%s'\n", *args);

	for (uint32_t j = 0; ssl_arg_flags[j].flag != NULL; j++)
	{
//		printf("compared with '%s'\n", ssl_arg_flags[j].flag);
		if (!ft_strcmp(ssl_arg_flags[j].flag, *args))
		{
			if (check_nb_args_required(args, ssl_arg_flags[j].nb_args) == true)
			{
				ssl_arg_flags[j].flag_handler(env, *args);
				return (ssl_arg_flags[j].nb_args);
			}
			else
				break ;
		}
	}
	return 0;
}

void						parse_md5(t_ssl_env *env, char **args)
{
	printf("parse_md5 called\n");
	(void)env;
	uint32_t				ret;

	for (uint32_t i = 1; args[i] != NULL; i++)
	{
		printf("arg : %s\n", args[i]);
		ret = check_arg_flags(env, &args[i]);
		if (ret == 0 && args[i][0] == '-')
			parse_noargs_flag(env, args[i]);
		else
			i += ret;
	}
}
