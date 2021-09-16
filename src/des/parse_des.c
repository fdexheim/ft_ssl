#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void					parse_flag_cluster(t_ssl_env *env, char *arg)
{
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
		env->flags.a = arg[i] == 'a' ? true : env->flags.a;
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
static void					handle_flag_p(t_ssl_env *env, char **args)
{
	env->flags.p = true;
	env->flags.p_arg = args[1];
}

//------------------------------------------------------------------------------
static void					handle_flag_k(t_ssl_env *env, char **args)
{
	env->flags.k = true;
	env->flags.k_arg = args[1];
}

//------------------------------------------------------------------------------
static void					handle_flag_v(t_ssl_env *env, char **args)
{
	env->flags.v = true;
	env->flags.v_arg = args[1];
}

//------------------------------------------------------------------------------
static uint32_t				check_arg_flags(t_ssl_env *env, char **args)
{
	const t_ssl_arg_flags	ssl_arg_flags[] = {
		{ "-i", 1, handle_flag_in },
		{ "-o", 1, handle_flag_out },
		{ "-s", 1, handle_flag_s },
		{ "-p", 1, handle_flag_p },
		{ "-k", 1, handle_flag_k },
		{ "-d", 0, handle_flag_d },
		{ "-e", 0, handle_flag_e },
		{ "-v", 1, handle_flag_v },
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

static void					handle_flag_ecb(t_ssl_env *env, char **args)
{
	// NYI
}

static void					handle_flag_cbc(t_ssl_env *env, char **args)
{
	// NYI
}

//------------------------------------------------------------------------------
static void					parse_specific_des_command(t_ssl_env *env, char **args)
{
	const t_ssl_arg_flags	specific_command[] = {
		{ "ecb", 0, handle_flag_ecb },
		{ "cbc", 0, handle_flag_cbc }
	};

	if (args[0] != NULL && !ft_strcmp(args[0], "des3"))
	{
		// set flag des3 here;
	}
	if (args[1] != NULL)
	{
		for (uint32_t j = 0; specific_command[j].flag != NULL; j++)
		{
			if (!ft_strcmp(specific_command[j].flag, args[1]))
			{
				specific_command[j].flag_handler(env, args);
			}
		}
	}
}

//------------------------------------------------------------------------------
void						parse_des(t_ssl_env *env, char **args)
{
	uint32_t				arg_flag_size;
	char					**arg0_split;
	char *tmp = args[0];

	while (*tmp)
	{
		if (*tmp == '-')
		{
			*tmp = ' ';
		}
	}
	arg0_split = ft_tokenizer(args[0]);

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
