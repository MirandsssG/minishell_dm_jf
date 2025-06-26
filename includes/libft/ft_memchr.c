/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dluis-ma <dluis-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 03:16:03 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/13 15:04:25 by dluis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*s_str;

	i = 0;
	s_str = (const unsigned char *)s;
	while (i < n)
	{
		if (s_str[i] == (unsigned char)c)
			return ((void *)(s_str + i));
		i++;
	}
	return (NULL);
}
