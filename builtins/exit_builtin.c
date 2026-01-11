/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:47 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/11 03:18:33 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}
static int	ft_strdigit(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (ft_isdigit((int)str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}
int	exit_builtin(t_data *data)
{
	int	status;
	t_cmd *cmd;

	cmd = data->cmd;
	status = 0;
	if (cmd->args[1] && cmd->args[2])
	{
		fprintf(stderr, "%s: too many arguments\n", cmd->args[0]);
		return (data->last_exit_status = 1, 1);
	}
	if (cmd->args[1] && ft_strdigit(cmd->args[1]) == 0)
	{
		fprintf(stderr, "exit: %s: numeric argument required\n", cmd->args[1]);
		return (2);
	}
	if (data->cmd->args[1])
		status = ft_atoi(cmd->args[1]);
	else
		status = data->last_exit_status;
	printf("exit\n");
	return (status);
}
