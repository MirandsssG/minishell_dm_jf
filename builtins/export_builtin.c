/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:32 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/11 03:22:46 by mirandsssg       ###   ########.fr       */
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

int	export_builtin(t_data *data, t_cmd *cmd)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal;
	char	**args;

	i = 1;
	args = cmd->args;
	if (!args[1])
	{
		print_export(data->env_list);
		return (0);
	}
	while (args[i])
	{
		if (!is_valid_varname(args[i]))
			printf("minishell: export: `%s`: not a valid varname\n", args[i]);
		else
		{
			equal = ft_strchr(args[i], '=');
			if (equal)
			{
				key = ft_substr(args[i], 0, equal - args[i]);
				value = ft_strdup(equal + 1);
			}
			else
			{
				key = ft_strdup(args[i]);
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
