#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void					parse_flag_cluster(t_ssl_env *env, char *arg)
{
	(void)env;
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
	
	}
}

//------------------------------------------------------------------------------
static void					handle_flag_d(t_ssl_env *env, char **args)
{
	(void)args;
	env->flags.d = true;
	if (env->flags.e == true)
	{
		ft_putstr("[Warning] You've set both encrypt and decrypt flags you dumbass\n");
		ft_putstr("[Warning] Defaulting to decrypt...\n");
		env->flags.e = false;
	}
}

//------------------------------------------------------------------------------
static void					handle_flag_e(t_ssl_env *env, char **args)
{
	(void)args;
	env->flags.e = true;
	if (env->flags.d == true)
	{
		ft_putstr("[Warning] You've set both encrypt and decrypt flags you dumbass\n");
		ft_putstr("[Warning] Defaulting to encrypt...\n");
		env->flags.d = false;
	}
}

//------------------------------------------------------------------------------
static void					handle_flag_out(t_ssl_env *env, char **args)
{
	env->flags.o = true;
	env->flags.file_arg_out = args[1];
}

//------------------------------------------------------------------------------
static void					handle_flag_in(t_ssl_env *env, char **args)
{
	env->flags.i = true;
	env->flags.file_arg = args[1];
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
		{ "-i", 1, handle_flag_in },
		{ "-o", 1, handle_flag_out },
		{ "-s", 1, handle_flag_s },
		{ "-d", 0, handle_flag_d },
		{ "-e", 0, handle_flag_e },
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
void						parse_base64(t_ssl_env *env, char **args)
{
	uint32_t				arg_flag_size;

	for (uint32_t i = 1; args[i] != NULL; i++)
	{
		arg_flag_size = check_arg_flags(env, &args[i]);
		if (arg_flag_size == 0)
		{
			if (args[i][0] == '-')
				parse_flag_cluster(env, args[i]);
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
