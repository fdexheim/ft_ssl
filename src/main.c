#include "../inc/ft_ssl.h"

//------------------------------------------------------------------------------
void				list_commands(void)
{
	ft_putstr("\nStandard commands:\n");
	ft_putstr("\nMessage Digest commands:\n");
	ft_putstr("md5\nsha224\nsha256\nsha512\n");
	ft_putstr("\nCipher commands:\n");
	ft_putstr("base64\n");
	ft_putstr("des\n");
	ft_putstr("des-cbc\n");
	ft_putstr("des-ecb\n");
}

//------------------------------------------------------------------------------
void				usage(void)
{
	ft_putstr("Usage: ./ft_ssl command [command opts] [command args]\n");
	list_commands();
	return ;
}

//------------------------------------------------------------------------------
void				close_env(t_ssl_env *env)
{
	free(env);
}

//------------------------------------------------------------------------------
t_ssl_env			*setup_env(int argc, char **argv)
{
	t_ssl_env		*env;

	if ((env = malloc(sizeof(t_ssl_env))) == NULL)
	{
		ft_putstr("[ERROR] Failed memory allocation\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(env, sizeof(t_ssl_env));
	env->argc = argc;
	env->argv = argv;
	return (env);
}

//------------------------------------------------------------------------------
static void			reset_flags(t_ssl_env *env)
{
	ft_bzero(&env->flags, sizeof(t_ssl_flags));
}

//------------------------------------------------------------------------------
static void			stdin_loop_mode(t_ssl_env *env)
{
	char			*input;
	char			**split_input;
	size_t			input_len;

	while (1)
	{
		ft_putstr("ft_ssl > ");
		input = ft_get_full_input();
		if (input == NULL)
			return ;
		input_len = ft_strlen(input);
		if (input_len < 1)
		{
			free(input);
			return ;
		}
		if (input[input_len - 1] == '\n')
			input[input_len - 1] = '\0';
		if (!ft_strcmp(input, "exit"))
		{
			free(input);
			return ;
		}
		split_input = ft_tokenizer(input);
		free(input);
		if (split_input == NULL)
			continue ;
		if (ft_get_string_table_size(split_input) < 1)
		{
			ft_free_string_tab(split_input);
			continue ;
		}
		parse_command(env, split_input);
		ft_free_string_tab(split_input);
		reset_flags(env);
		env->file_args = NULL;
	}
}

//------------------------------------------------------------------------------
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
	close_env(env);
	return (EXIT_SUCCESS);
}
