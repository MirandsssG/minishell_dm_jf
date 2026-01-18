/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:32 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/18 18:34:30 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_env_node(t_env **env_list, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

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

void	update_env(t_env **env_list, char *key, char *value)
{
	t_env	*tmp;

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
			return ;
		}
		tmp = tmp->next;
	}
	add_env_node(env_list, key, value);
}

static void	handle_export_arg(t_data *data, char *arg)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key = ft_substr(arg, 0, equal - arg);
		value = ft_strdup(equal + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	update_env(&data->env_list, key, value);
	free(key);
	free(value);
}

int	export_builtin(t_data *data, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd->args[1])
	{
		print_export(data->env_list);
		return (0);
	}
	i = 1;
	ret = 0;
	while (cmd->args[i])
	{
		if (!is_valid_varname(cmd->args[i]))
		{
			printf("minishell: export: `%s`: not a valid varname\n",
				cmd->args[i]);
			ret = 1;
		}
		else
			handle_export_arg(data, cmd->args[i]);
		i++;
	}
	return (ret);
}
