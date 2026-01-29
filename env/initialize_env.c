/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:19:53 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/29 01:00:54 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*init_default_env(void)
{
	t_env	*head;
	t_env	*last;
	char	*cwd;

	head = NULL;
	last = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	if (add_default_var(&head, &last, "PWD", cwd))
		return (free(cwd), NULL);
	free(cwd);
	if (add_default_var(&head, &last, "SHLVL", "1"))
		return (free_env_list(head), NULL);
	if (add_default_var(&head, &last,
			"PATH", "/usr/local/bin:/usr/bin:/bin"))
		return (free_env_list(head), NULL);
	if (add_default_var(&head, &last, "_", "/usr/bin/env"))
		return (free_env_list(head), NULL);
	return (head);
}

static char	*handle_shlvl(char *key, char *value)
{
	int	lvl;

	if (ft_strcmp(key, "SHLVL") != 0)
		return (value);
	lvl = ft_atoi(value);
	if (lvl < 0)
		lvl = 0;
	free(value);
	return (ft_itoa(lvl + 1));
}

static int	append_env_node(
	t_env **head, t_env **last, char *key, char *value)
{
	t_env	*new;

	new = new_env_node(key, value);
	if (!new)
		return (1);
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
	return (0);
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*last;
	char	*key;
	char	*value;

	head = NULL;
	last = NULL;
	if (!envp || !*envp)
		return (init_default_env());
	while (*envp)
	{
		key = get_key(*envp);
		value = get_value(*envp);
		value = handle_shlvl(key, value);
		if (append_env_node(&head, &last, key, value))
			return (free(key), free(value),
				free_env_list(head), NULL);
		free(key);
		free(value);
		envp++;
	}
	return (head);
}
