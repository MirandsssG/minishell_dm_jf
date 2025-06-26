/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:25:18 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/08 04:52:57 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int nbr)
{
	if (nbr < 0)
		nbr = -nbr;
	return (nbr);
}

static size_t	ft_numlen(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		is_neg;
	size_t	length;

	length = ft_numlen(n);
	str = (char *)ft_calloc(length + 1, sizeof(char));
	if (!(str))
		return (NULL);
	is_neg = (n < 0);
	if (n == 0)
		str[0] = '0';
	while (n != 0)
	{
		str[--length] = ft_abs(n % 10) + '0';
		n = (n / 10);
	}
	if (is_neg)
		str[0] = '-';
	return (str);
}
