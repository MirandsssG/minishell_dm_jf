/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:19:47 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/11 18:05:35 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
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

static int	exit_checks(t_data *data, t_cmd *cmd)
{
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": to many arguments\n", 2);
		data->last_exit_status = 1;
		return (1);
	}
	if (cmd->args[1] && ft_strdigit(cmd->args[1]) == 0)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		data->should_exit = 1;
		printf("exit\n");
		return (data->last_exit_status = 2, 2);
	}
	return (0);
}

int	exit_builtin(t_data *data, t_cmd *cmd)
{
	int	status;
	int	ret;

	if (!cmd)
		cmd = data->cmd;
	status = 0;
	ret = exit_checks(data, cmd);
	if (ret != 0)
		return (data->last_exit_status);
	if (cmd->args && cmd->args[1])
		status = ft_atoi(cmd->args[1]);
	else
		status = data->last_exit_status;
	data->should_exit = 1;
	printf("exit\n");
	return (status);
}
