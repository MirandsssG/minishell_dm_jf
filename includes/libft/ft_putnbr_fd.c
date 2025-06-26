/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dluis-ma <dluis-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:26:49 by mirandsssg        #+#    #+#             */
/*   Updated: 2024/11/07 14:17:10 by dluis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int n)
{
	if (n < 0)
		n = -n;
	return (n);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	str[13];
	int		is_neg;
	int		lenght;

	is_neg = (n < 0);
	lenght = 0;
	ft_bzero(str, 13);
	if (n == 0)
		str[0] = '0';
	while (n != 0)
	{
		str[lenght] = ft_abs(n % 10) + '0';
		n = (n / 10);
		lenght++;
	}
	if (is_neg)
		str[lenght] = '-';
	else if (lenght > 0)
		lenght--;
	while (lenght >= 0)
		write(fd, &str[lenght--], 1);
}
