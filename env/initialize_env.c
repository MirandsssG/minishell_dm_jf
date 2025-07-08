/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:19:53 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/08 16:41:10 by mirandsssg       ###   ########.fr       */
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
		return (NULL);
	return (ft_strdup(equal_sign + 1));
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
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
