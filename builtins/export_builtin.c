/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:32 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/09 20:29:27 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_varname(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("declare -x %s", tmp->key);
		if (tmp->value)
			printf("=\"%s\"", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
}

void	update_env(t_env **env_list, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = NULL;
			return;
		}
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	if (!*env_list)
		*env_list = new;
	else
	{
		tmp = *env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	export_builtin(t_data *data)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal;
	char	**tokens;

	i = 1;
	tokens = data->tokens;
	if (!tokens[1])
	{
		print_export(data->env_list);
		return (0);
	}
	while (tokens[i])
	{
		if (!is_valid_varname(tokens[i]))
			printf("minishell: export: `%s`: not a valid varname\n", tokens[i]);
		else
		{
			equal = ft_strchr(tokens[i], '=');
			if (equal)
			{
				key = ft_substr(tokens[i], 0, equal - tokens[i]);
				value = ft_strdup(equal + 1);
			}
			else
			{
				key = ft_strdup(tokens[i]);
				value = NULL;
			}
			update_env(&data->env_list, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
}
