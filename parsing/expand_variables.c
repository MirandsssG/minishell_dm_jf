/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:26:41 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/29 00:40:06 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*remove_quotes(const char *str)
{
	char	*res;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	res = malloc(ft_strlen(str) + 1);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

static char	*expand_token(const char *token, t_data *data)
{
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	data->in_single = 0;
	data->in_double = 0;
	result = ft_strdup("");
	while (token[i])
		process_char(token, data, &result, &i);
	temp = remove_quotes(result);
	free(result);
	return (temp);
}

static int	is_heredoc_delimiter(char **tokens, int i)
{
	if (i > 0 && ft_strcmp(tokens[i - 1], "<<") == 0)
		return (1);
	return (0);
}

void	expand_variables(t_data *data)
{
	int		i;
	char	*expanded;

	i = 0;
	while (data->tokens[i])
	{
		if (is_heredoc_delimiter(data->tokens, i))
		{
			i++;
			continue ;
		}
		expanded = expand_token(data->tokens[i], data);
		free(data->tokens[i]);
		data->tokens[i] = expanded;
		i++;
	}
}
