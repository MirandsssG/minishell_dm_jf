/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 12:09:26 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/12 12:16:12 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_tokens(t_data *data)
{
	if (data->tokens)
	{
		free_split(data->tokens);
		data->tokens = NULL;
	}
	data->tokens = malloc(sizeof(char *) * 1024);
	data->i = 0;
	data->j = 0;
	data->k = 0;
}

int	handle_double_operator(t_data *data)
{
	if ((data->input[data->i] == '>' && data->input[data->i + 1] == '>')
		|| (data->input[data->i] == '<' && data->input[data->i + 1] == '<'))
	{
		data->tokens[data->k] = ft_substr(data->input, data->i, 2);
		data->k++;
		data->i += 2;
		return (1);
	}
	return (0);
}

int	handle_single_operator(t_data *data)
{
	if (is_special(data->input[data->i]))
	{
		data->tokens[data->k] = ft_strndup(&data->input[data->i], 1);
		data->k++;
		data->i++;
		return (1);
	}
	return (0);
}

char	*read_part(t_data *data)
{
	char	quote;
	int		start;

	if (data->input[data->i] == '\'' || data->input[data->i] == '"')
	{
		quote = data->input[data->i++];
		start = data->i - 1;
		while (data->input[data->i] && data->input[data->i] != quote)
			data->i++;
		if (data->input[data->i] == quote)
			data->i++;
		return (ft_substr(data->input, start, data->i - start));
	}
	start = data->i;
	while (data->input[data->i]
		&& !is_space(data->input[data->i])
		&& !is_special(data->input[data->i])
		&& data->input[data->i] != '\''
		&& data->input[data->i] != '"')
		data->i++;
	return (ft_strndup(&data->input[start], data->i - start));
}

void	handle_word(t_data *data)
{
	char	*tmp;
	char	*part;
	char	*old;

	tmp = NULL;
	while (data->input[data->i]
		&& !is_space(data->input[data->i])
		&& !is_special(data->input[data->i]))
	{
		part = read_part(data);
		if (!tmp)
			tmp = part;
		else
		{
			old = tmp;
			tmp = ft_strjoin(old, part);
			free(old);
			free(part);
		}
	}
	data->tokens[data->k++] = tmp;
}
