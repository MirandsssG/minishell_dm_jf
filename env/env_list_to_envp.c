/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_envp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:19:02 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/11 02:29:52 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return;
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

char	**env_list_to_envp(t_env *env_list)
{
	int		i;
	int		count;
	char	**envp;
	char	*joined;
	size_t	key_len;
	size_t	value_len;

	i = 0;
	count = count_env_vars(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	while (env_list)
	{
		key_len = ft_strlen(env_list->key);
		value_len = ft_strlen(env_list->value);
		joined = malloc(key_len + value_len + 2);
		if (!joined)
		{
			free_envp(envp);
			return (NULL);
		}
		ft_strcpy(joined, env_list->key);
		joined[key_len] = '=';
		ft_strcpy(joined + key_len + 1, env_list->value);
		envp[i++] = joined;
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}
