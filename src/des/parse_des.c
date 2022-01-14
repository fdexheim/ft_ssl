#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
static void					parse_flag_cluster(t_ssl_env *env, char *arg)
{
	for (uint32_t i = 1; arg[i] != '\0'; i++)
	{
		env->flags.a = arg[i] == 'a' ? true : env->flags.a;
		env->flags.P = arg[i] == 'P' ? true : env->flags.P;
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

//------------------------------------------------------------------------------
static e_des_operating_mode		parse_specific_des_command(char **args)
{
	const t_des_operating_mode		modes[] = {
		{ "cbc", CBC },
		{ "ecb", ECB },
		{ NULL, 0 }
	};
	e_des_operating_mode		ret = 0;

	if (args[0] != NULL && !ft_strcmp(args[0], "des3"))
		ret |= DES3 ;
	if (args[1] == NULL)
	{
		ret |= CBC;
		return ret;
	}
	for (uint32_t j = 0; modes[j].mode != NULL; j++)
		if (!ft_strcmp(modes[j].mode, args[1]))
			ret |= modes[j].value;
	return ret;
}

//------------------------------------------------------------------------------
e_des_operating_mode	parse_des(t_ssl_env *env, char **args)
{
	uint32_t				arg_flag_size;
	char					**arg0_split;
	char *tmp = args[0];
	e_des_operating_mode	op_flags = 0;

	while (*tmp)
	{
		if (*tmp == '-')
			*tmp = ' ';
		tmp++;
	}
	arg0_split = ft_tokenizer(args[0]);
	op_flags |= parse_specific_des_command(arg0_split);
	ft_free_string_tab(arg0_split);

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
					return op_flags ;
				}
			}
		}
		else
			i += arg_flag_size;
	}
	return (op_flags);
}
