/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_valid_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdexheim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 11:26:36 by fdexheim          #+#    #+#             */
/*   Updated: 2019/08/14 13:02:15 by fdexheim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

bool				ft_valid_quote(char *s)
{
	char			quote;

	quote = *s;
	s += 1;
	while (*s)
	{
		if (*s == quote)
		{
			if (*(s + 1) == ' ' || *(s + 1) == '\0')
				return (true);
			else
				return (false);
		}
		s++;
	}
	return (true);
}
