/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:39 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/14 19:21:05 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_env_var(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	unset_builtin(t_data *data, t_cmd *cmd)
{
	int		i;
	char	**args;
	int		ret;

	ret = 0;
	args = cmd->args;
	i = 1;
	while (args[i])
	{
		if (!is_valid_varname(args[i]))
		{
			printf("minishell: unset: `%s`: not a valid varname\n", args[i]);
			ret = 1;
		}
		else
			remove_env_var(&data->env_list, args[i]);
		i++;
	}
	return (ret);
}
