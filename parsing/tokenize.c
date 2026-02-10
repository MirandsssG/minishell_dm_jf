/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:23:32 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/10 15:09:58 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

void	tokenize_inputs(t_data *data)
{
	init_tokens(data);
	while (data->input[data->i])
	{
		while (is_space(data->input[data->i]))
			data->i++;
		if (!data->input[data->i])
			break ;
		if (handle_double_operator(data))
			continue ;
		if (handle_single_operator(data))
			continue ;
		handle_word(data);
	}
	data->tokens[data->k] = NULL;
}
