#include "../inc/ft_ssl.h"

void						parse_command(t_ssl_env *env, char **args)
{
	const t_ssl_command		cmds[] = {
		{ "md5", md5_command },
		{ "sha256", sha256_command },
		{ NULL, NULL }
	};
	uint32_t				index = 0;

	while (cmds[index].command_name != NULL)
	{
		if (!ft_strcmp(args[0], cmds[index].command_name))
		{
			env->command = cmds[index].command;
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
		env->command(env, args);
}
