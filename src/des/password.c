#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"
#include "../inc/ft_ssl_des.h"
#ifdef __APPLE__
	#include <readpassphrase.h>
	#include <sys/random.h>
#else
	#include <bsd/readpassphrase.h>
#endif

//------------------------------------------------------------------------------
char*					get_password(t_ssl_env *env)
{
	size_t				buff_size = 256;
	char				buff_verif[buff_size];
	char				*buff = NULL;

	if (env->flags.p == true)
		return (ft_strdup(env->flags.p_arg));
	if ((buff = malloc(sizeof(char) * buff_size)) == NULL)
		return (NULL);
	ft_bzero(buff, buff_size);
	ft_bzero(buff_verif, buff_size);
	if (env->flags.k == false)
	{
		if (env->flags.d == true)
			readpassphrase("Enter DES decryption password :", buff, buff_size, 0);
		else
			readpassphrase("Enter DES encryption password :", buff, buff_size, 0);
		if (env->flags.d == false)
		{
			readpassphrase("Verifying - Enter DES encryption password :", buff_verif, buff_size, 0);
			if (ft_strcmp(buff, buff_verif))
			{
				ft_putstr("[Error] password verification failed\n");
				free(buff);
				return (NULL);
			}
		}
		if (ft_strlen(buff) == 0)
		{
			ft_putstr("[Error] Bad password read\n");
			free(buff);
			return (NULL);
		}
	}
	return (buff);
}
