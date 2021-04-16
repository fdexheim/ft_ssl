#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void				display_after_sha256(char *src, bool string_mode)
{
	if (string_mode == true)
	{
		ft_putstr(" \"");
		ft_putstr(src);
		ft_putstr("\"");
	}
	else if (src != NULL)
	{
		ft_putstr(" ");
		ft_putstr(src);
	}
}

//------------------------------------------------------------------------------
static void				display_before_sha256(char *src, bool string_mode)
{
	if (string_mode == true)
	{
		ft_putstr("SHA256(\"");
		ft_putstr(src);
		ft_putstr("\")= ");
	}
	else if (src != NULL)
	{
		ft_putstr("SHA256(");
		ft_putstr(src);
		ft_putstr(")= ");
	}
	else
	{
		ft_putstr("(stdin)= ");
	}
}

//------------------------------------------------------------------------------
void				display_sha256(t_ssl_env *env, char *src, void *state, bool string_mode)
{
	if (env->flags.q != true && env->flags.r == false)
	{
		display_before_sha256(src, string_mode);
	}
	display_hash(state, 32);
	if (env->flags.q != true && env->flags.r == true)
	{
		display_after_sha256(src, string_mode);
	}
	ft_putstr("\n");
}
