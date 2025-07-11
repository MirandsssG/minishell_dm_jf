/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:26:41 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/11 19:21:29 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_token(const char *token, t_data *data)
{
	char	*result;
	char	*temp;
	int		start;
	char	*var_name;
	char	*value;
	int		i;

	i = 0;
	data->in_single = 0;
	data->in_double = 0;
	result = ft_strdup("");
	while (token[i])
	{
		if (token[i] == '\'' && !data->in_double)
		{
			data->in_single = !data->in_single;
			result = ft_strjoin_free_expand(result, ft_strndup(&token[i++], 1), 1, 1);
		}
		else if (token[i] == '\"' && !data->in_single)
		{
			data->in_double = !data->in_double;
			result = ft_strjoin_free_expand(result, ft_strndup(&token[i++], 1), 1, 1);
		}
		else if (token[i] == '$' && !data->in_single)
		{
			if (token[i + 1] == '?')
			{
				temp = ft_itoa(data->last_exit_status);
				result = ft_strjoin_free_expand(result, temp, 1, 1);
				i += 2;
			}
			else if (token[i + 1] == '$')
			{
				temp = ft_itoa(getpid());
				result = ft_strjoin_free_expand(result, temp, 1, 1);
				i += 2;
			}
			else if (ft_isalpha(token[i + 1]) || token[i + 1] == '_')
			{
				start = ++i;
				while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
					i++;
				var_name = ft_strndup(&token[start], i - start);
				value = getenv(var_name);
				free(var_name);
				if (value)
					result = ft_strjoin_free_expand(result, ft_strdup(value), 1, 1);
			}
			else
				result = ft_strjoin_free_expand(result, ft_strndup(&token[i++], 1), 1, 1);
		}
		else
			result = ft_strjoin_free_expand(result, ft_strndup(&token[i++], 1), 1, 1);
	}
	return (result);
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
