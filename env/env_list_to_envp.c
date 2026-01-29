/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_envp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:19:02 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/29 01:04:35 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static int	count_env_vars(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

static char	*join_env_var(char *key, char *value)
{
	char	*joined;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	joined = malloc(key_len + value_len + 2);
	if (!joined)
		return (NULL);
	ft_strcpy(joined, key);
	joined[key_len] = '=';
	ft_strcpy(joined + key_len + 1, value);
	return (joined);
}

char	**env_list_to_envp(t_env *env_list)
{
	int		i;
	int		count;
	char	**envp;

	i = 0;
	count = count_env_vars(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	while (env_list)
	{
		envp[i] = join_env_var(env_list->key, env_list->value);
		if (!envp[i])
			return (free_envp(envp), NULL);
		i++;
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}
