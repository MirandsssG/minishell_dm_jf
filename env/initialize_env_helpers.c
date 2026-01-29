/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:56:19 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/29 01:00:12 by mirandsssg       ###   ########.fr       */
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

int	add_default_var(
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
