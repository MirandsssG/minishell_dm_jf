/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 03:15:34 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/07 17:47:50 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str_s;

	i = 0;
	str_s = (char *)s;
	while (str_s[i] != '\0')
		i++;
	if (c == '\0')
		return (&str_s[i]);
	while (i >= 0)
	{
		if (str_s[i] == (char)c)
			return (&str_s[i]);
		i--;
	}
	return (NULL);
}
