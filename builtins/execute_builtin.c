/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:22:14 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/06/22 14:53:28 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_data *data)
{
	if (!data->tokens || !data->tokens[0])
		return (1);
	if (ft_strcmp(data->tokens[0], "cd") == 0)
		return (cd_builtin(data));
	if (ft_strcmp(data->tokens[0], "echo") == 0)
		return (echo_builtin(data));
	if (ft_strcmp(data->tokens[0], "pwd") == 0)
		return (pwd_builtin(data));
	if (ft_strcmp(data->tokens[0], "export") == 0)
		return (export_builtin(data));
	if (ft_strcmp(data->tokens[0], "unset") == 0)
		return (unset_builtin(data));
	if (ft_strcmp(data->tokens[0], "env") == 0)
		return (env_builtin(data));
	if (ft_strcmp(data->tokens[0], "exit") == 0)
		return (exit_builtin(data));
	return (0);
}
