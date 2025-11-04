/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:19:53 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/11/04 17:55:10 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_key(char *env_str)
{
	int	i;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	return (ft_strndup(env_str, i));
}

char	*get_value(char *env_str)
{
	char	*equal_sign;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (ft_strdup(""));
	return (ft_strdup(equal_sign + 1));
}

char	*get_env_value(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	t_env	*head = NULL;
	t_env	*last = NULL;
	t_env	*new;
	char	*key;
	char	*value;
	char	*cwd;
	
	if (!envp || !*envp)
	{
		cwd = getcwd(NULL, 0);
		head = new_env_node("PWD", cwd);
		free(cwd);
		if (!head)
			return (NULL);
		last = head;
		new = new_env_node("SHLVL", "1");
		if (!new)
			return (head);
		last->next = new;
		last = new;
		new = new_env_node("PATH", "/usr/local/bin:/usr/bin:/bin");
		if (!new)
			return (head);
		last->next = new;
		last = new;
		new = new_env_node("_", "/usr/bin/env");
		if (!new)
			return (head);
		last->next = new;
		return (head);
	}
	
	while (*envp)
	{
		key = get_key(*envp);
		value = get_value(*envp);
		new = new_env_node(key, value);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
		envp++;
	}
	return (head);
}
