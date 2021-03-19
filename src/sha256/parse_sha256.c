#include "../../inc/ft_ssl.h"

void				parse_sha256(t_ssl_env *env, char **args)
{
	(void)env;

	for (char **tmp = args; tmp != NULL; tmp++)
	{
		printf("arg : %s\n", *tmp);
	}
}
