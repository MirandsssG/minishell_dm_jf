/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 11:54:13 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/10 16:15:36 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_quotes(char c, t_data *data)
{
	if (c == '\'' && !data->in_double)
	{
		data->in_single = !data->in_single;
		return (1);
	}
	if (c == '\"' && !data->in_single)
	{
		data->in_double = !data->in_double;
		return (1);
	}
	return (0);
}

int	handle_special_dollar(const char *token, t_data *data,
									char **result, int *i)
{
	char	*temp;

	if (token[*i + 1] == '?')
	{
		temp = ft_itoa(data->last_exit_status);
		*result = ft_strjoin_free_expand(*result, temp, 1, 1);
		*i += 2;
		return (1);
	}
	if (token[*i + 1] == '$')
	{
		temp = ft_itoa(getpid());
		*result = ft_strjoin_free_expand(*result, temp, 1, 1);
		*i += 2;
		return (1);
	}
	return (0);
}

int	handle_env_var(const char *token, t_data *data,
							char **result, int *i)
{
	int		start;
	char	*var_name;
	char	*value;

	if (!ft_isalpha(token[*i + 1]) && token[*i + 1] != '_')
		return (0);
	start = ++(*i);
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(&token[start], *i - start);
	value = get_env_value(var_name, data->env_list);
	free(var_name);
	if (value)
		*result = ft_strjoin_free_expand(*result, ft_strdup(value), 1, 1);
	return (1);
}

void	process_char(const char *token, t_data *data,
							char **result, int *i)
{
	char	c;

	c = token[*i];
    if (handle_quotes(c, data))
	{
		*result = ft_strjoin_free_expand(
			*result, ft_strndup(&token[*i], 1), 1, 1);
		(*i)++;
		return;
	}
    if (c == '$' && !data->in_single)
    {
        if (handle_special_dollar(token, data, result, i))
            return;
        if (handle_env_var(token, data, result, i))
            return;
        *result = ft_strjoin_free_expand(*result, ft_strndup(&token[(*i)++], 1), 1, 1);
        return;
    }
    *result = ft_strjoin_free_expand(*result, ft_strndup(&token[(*i)++], 1), 1, 1);
}
void	remove_token_index(char **tokens, int index)
{
	int	i;

	i = index;
	while (tokens[i + 1])
	{
		tokens[i] = tokens[i + 1];
		i++;
	}
	tokens[i] = NULL;
}
