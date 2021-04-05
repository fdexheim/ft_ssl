#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void					parse_noargs_flag(t_ssl_env *env, char *arg)
{
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
		env->flags.p = arg[i] == 'p' ? true : env->flags.p;
		env->flags.q = arg[i] == 'q' ? true : env->flags.q;
		env->flags.r = arg[i] == 'r' ? true : env->flags.r;
	}
}

//------------------------------------------------------------------------------
static void					handle_flag_s(t_ssl_env *env, char **args)
{
	env->flags.s = true;
	env->flags.s_arg = args[1];
}

//------------------------------------------------------------------------------
static uint32_t				check_arg_flags(t_ssl_env *env, char **args)
{
	const t_ssl_arg_flags	ssl_arg_flags[] = {
		{ "-s", 1, handle_flag_s },
		{ NULL, 0, NULL }
	};

	for (uint32_t j = 0; ssl_arg_flags[j].flag != NULL; j++)
	{
		if (!ft_strcmp(ssl_arg_flags[j].flag, *args))
		{
			if (check_nb_args_required(args, ssl_arg_flags[j].nb_args) == true)
			{
				ssl_arg_flags[j].flag_handler(env, args);
				return (ssl_arg_flags[j].nb_args);
			}
			else
				break ;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
void						parse_md5(t_ssl_env *env, char **args)
{
	uint32_t				arg_flag_size;

	for (uint32_t i = 1; args[i] != NULL; i++)
	{
		arg_flag_size = check_arg_flags(env, &args[i]);
		if (arg_flag_size == 0)
		{
			if (args[i][0] == '-')
				parse_noargs_flag(env, args[i]);
			else
			{
				if (env->file_args == NULL)
				{
					env->file_args = &args[i];
					return ;
				}
			}
		}
		else
			i += arg_flag_size;
	}
}
