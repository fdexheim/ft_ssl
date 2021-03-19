#include "../../inc/ft_ssl.h"

void			sha256_command(t_ssl_env *env, char **args)
{
	(void)env;
	printf("sha_command called\n");
	parse_sha256(env, args);
}
