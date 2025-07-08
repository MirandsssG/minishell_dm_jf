/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:18 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/08 16:08:16 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	echo_builtin(t_data *data)
{
	int		i;
	int		j;
	bool	n_flag;

	i = 1;
	n_flag = false;
	while (data->tokens[i] && data->tokens[i][0] == '-' && data->tokens[i][1] == 'n')
	{
		j = 2;
		while (data->tokens[i][j] == 'n')
			j++;
		if (data->tokens[i][j] != '\0')
			break;
		n_flag = true;
		i++;
	}
	while (data->tokens[i])
	{
		printf("%s", data->tokens[i]);
		if (data->tokens[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
