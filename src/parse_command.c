#include "../inc/ft_ssl.h"

//------------------------------------------------------------------------------
bool						check_nb_args_required(char **args, uint32_t nb_arg)
{
	for (uint32_t i = 1; i <= nb_arg; i++)
	{
		if (args[i] == NULL)
		{
			printf("Missing arguments for flag %s\n", args[0]);
			return (false);
		}
	}
	return (true);
}

//------------------------------------------------------------------------------
void						parse_command(t_ssl_env *env, char **args)
{
	uint32_t				index = 0;

	env->full_command = args;
	while (g_commands[index].command_name != NULL)
	{
		if (!ft_strcmp(args[0], g_commands[index].command_name))
		{
			env->command = g_commands[index].command;
			break ;
		}
		index++;
	}
	if (env->command == NULL)
	{
		printf("%s: Error: '%s' is an invalid command\n", env->argv[0],
			args[0]);
		usage();
	}
	else
	{
		env->command(env, args);
	}
}
