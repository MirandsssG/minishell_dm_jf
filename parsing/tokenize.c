/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:23:32 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/06/22 12:18:03 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

void	tokenize_inputs(t_data *data)
{
	data->tokens = malloc(sizeof(char *) * 1024);
	data->i = 0;
	data->j = 0;
	data->k = 0;
	while (data->input[data->i])
	{
		while (is_space(data->input[data->i]))
			data->i++;
		if (!data->input[data->i]) break;
		if (is_special(data->input[data->i]))
		{
			data->tokens[data->k] = malloc(2);
			data->tokens[data->k][0] = data->input[data->i];
			data->tokens[data->k][1] = '\0';
			data->k++;
			data->i++;
		}
		else
		{
			data->j = data->i;
			while (data->input[data->i] && !is_space(data->input[data->i]) && !is_special(data->input[data->i]))
				data->i++;
			data->tokens[data->k] = ft_strndup(&data->input[data->j], data->i - data->j);
			data->k++;
		}
	}
	data->tokens[data->k] = NULL;
}
