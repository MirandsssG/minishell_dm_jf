/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:23:32 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/11/04 16:43:08 by mirandsssg       ###   ########.fr       */
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
	char	quote;
	char	*tmp;
	char	*part;
	char	*old;

	if (data->tokens)
	{
		free_split(data->tokens);
		data->tokens = NULL;
	}
	data->tokens = malloc(sizeof(char *) * 1024);
	data->i = 0;
	data->j = 0;
	data->k = 0;
	while (data->input[data->i])
	{
		while (is_space(data->input[data->i]))
			data->i++;
		if (!data->input[data->i]) 
			break;
		if ((data->input[data->i] == '>' && data->input[data->i + 1] == '>') ||
			(data->input[data->i] == '<' && data->input[data->i + 1] == '<'))
		{
			data->tokens[data->k] = malloc(3);
			data->tokens[data->k][0] = data->input[data->i];
			data->tokens[data->k][1] = data->input[data->i + 1];
			data->tokens[data->k][2] = '\0';
			data->k++;
			data->i += 2;
}
		else if (is_special(data->input[data->i]))
		{
			data->tokens[data->k] = malloc(2);
			data->tokens[data->k][0] = data->input[data->i];
			data->tokens[data->k][1] = '\0';
			data->k++;
			data->i++;
		}
		else
		{
			tmp = NULL;
			while (data->input[data->i] && !is_space(data->input[data->i]) && !is_special(data->input[data->i]))
			{
				if (data->input[data->i] == '\'' || data->input[data->i] == '"')
				{
					quote = data->input[data->i++];
					data->j = data->i;
					while (data->input[data->i] && data->input[data->i] != quote)
						data->i++;
					part = ft_strndup(&data->input[data->j], data->i - data->j);
					data->i++;
				}
				else
				{
					data->j = data->i;
					while (data->input[data->i] && !is_space(data->input[data->i]) && !is_special(data->input[data->i])
						&& data->input[data->i] != '\'' && data->input[data->i] != '"')
						data->i++;
					part = ft_strndup(&data->input[data->j], data->i - data->j);
				}
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
	}
	data->tokens[data->k] = NULL;
}
