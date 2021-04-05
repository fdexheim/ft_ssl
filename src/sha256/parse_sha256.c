#include "../../inc/ft_ssl.h"

/*
//------------------------------------------------------------------------------
static void						parse_noargs_flag(t_ssl_env *env, char *arg)
{
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
		env->flags.p = arg[i] == 'p' ? true : env->flags.p;
		env->flags.q = arg[i] == 'q' ? true : env->flags.q;
		env->flags.r = arg[i] == 'r' ? true : env->flags.r;
		env->flags.s = arg[i] == 's' ? true : env->flags.s;
	}
}

//------------------------------------------------------------------------------
static uint32_t				check_arg_flags(t_ssl_env *env, char **args)
{
	const t_ssl_arg_flags	ssl_arg_flags[] = {
		{ NULL, 0, NULL }
	};

	for (uint32_t j = 0; ssl_arg_flags[j].flag != NULL; j++)
	{
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

*/
//------------------------------------------------------------------------------
void						parse_sha256(t_ssl_env *env, char **args)
{
	(void)env;
	(void)args;
}

