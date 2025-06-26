/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 03:14:39 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/07 17:12:18 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*s_dest;
	char	*s_src;

	if (n == 0 || dest == src)
		return (dest);
	if (!dest && !src)
		return (NULL);
	i = 0;
	s_dest = (char *)dest;
	s_src = (char *)src;
	while (i < n)
	{
		s_dest[i] = s_src[i];
		i++;
	}
	return (s_dest);
}
