/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 03:15:23 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/08 04:11:54 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str_s;

	i = 0;
	str_s = (char *)s;
	while (str_s[i] != '\0')
	{
		if (str_s[i] == (char)c)
			return (&str_s[i]);
		i++;
	}
	if ((char)c == '\0')
		return (&str_s[i]);
	return (NULL);
}
