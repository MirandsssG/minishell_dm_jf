/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:26:41 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/06/22 13:04:21 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_token(const char *token, t_data *data)
{
	char	*result;
	char	*value;
	char	*value_ptr;
	char	*final;

	result = malloc(4096);
	if (!result)
		return (NULL);
	data->i = 0;
	data->j = 0;
	while (token[data->i])
	{
		if (token[data->i] == '$' && token[data->i + 1] && token[data->i + 1] != ' ' && token[data->i + 1] != '$')
		{
			data->i++;
			data->var_start = data->i;
			while (token[data->i] && (ft_isalnum(token[data->i]) || token[data->i] == '_'))
				data->i++;
			data->var_name = ft_strndup(&token[data->var_start], data->i - data->var_start);
			if (!data->var_name)
				return (NULL);
			value = getenv(data->var_name);
			free(data->var_name);
			if (value)
			{
				value_ptr = value;
				while (*value_ptr)
					result[data->j++] = *value_ptr++;
			}
			else
			{
				result[data->j++] = '$';
				data->var_i = data->var_start;
				while (data->var_i < data->i)
					result[data->j++] = token[data->var_i++];
			}
		}
		else
			result[data->j++] = token[data->i++];
	}
	result[data->j] = '\0';
	final = ft_strdup(result);
	free(result);
	return (final);
}

void	expand_variables(t_data *data)
{
	int		i;
	char	*expanded;

	i = 0;
	while (data->tokens[i])
	{
		expanded = expand_token(data->tokens[i], data);
		free(data->tokens[i]);
		data->tokens[i] = expanded;
		i++;
	}
}
