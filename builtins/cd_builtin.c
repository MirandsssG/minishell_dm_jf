/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:18:50 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/08 16:47:22 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

void	add_env_var(t_env **env_list, const char *key, const char *value)
{
	t_env *tmp = *env_list;

	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return;
		}
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	t_env *new = create_env_node(key, value);
	if (!new)
		return;
	if (!*env_list)
		*env_list = new;
	else
		tmp->next = new;
}

void	update_env(t_data *data, char *key, char *value)
{
	t_env *current = data->env_list;
	
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	add_env_var(&data->env_list, key, value);
}

int	cd_builtin(t_data *data)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!data->tokens[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			free(old_pwd);
			return (1);
		}
	}
	else
		path = data->tokens[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("getcwd");
		free(old_pwd);
		return (1);
	}
	update_env(data, "OLDPWD", old_pwd);
	update_env(data, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}
