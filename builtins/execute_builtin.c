/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:22:14 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/13 19:05:13 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_data *data, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_builtin(data, cmd));
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_builtin(cmd));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (pwd_builtin());
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (export_builtin(data, cmd));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset_builtin(data, cmd));
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (env_builtin(data));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_builtin(data, cmd));
	return (0);
}

static int save_stdio(int *in_save, int *out_save)
{
	*in_save = dup(STDIN_FILENO);
	*out_save = dup(STDOUT_FILENO);
	if (*in_save == -1 || *out_save == -1)
	{
		perror("dup");
		if (*in_save != -1)
			close(*in_save);
		if (*out_save != -1)
			close(*out_save);
		return (-1);
	}
	return (0);
}

static void restore_stdio(int in_save, int out_save)
{
	if (dup2(in_save, STDIN_FILENO) == -1)
		perror("dup2 restore stdin");
	if (dup2(out_save, STDOUT_FILENO) == -1)
		perror("dup2 restore stdout");
	close(in_save);
	close(out_save);
}

static int setup_redirections(t_data *data, t_cmd *cmd, int *ret_on_abort)
{
	if (process_heredocs(cmd, data) == -1)
	{
		*ret_on_abort = data->last_exit_status;
		if (cmd && cmd->infile_fd > 0)
		{
			close(cmd->infile_fd);
			cmd->infile_fd = -1;
		}
		return (-1);
	}
	if (heredoc_infile(cmd) != 0)
		return (-1);
	if (redirection_infile(cmd) != 0)
		return (-1);
	if (redirection_outfile(cmd) != 0)
		return (-1);
	return (0);
}
int	execute_builtin_with_redirections(t_data *data, t_cmd *cmd)
{
	int stdin_copy;
	int stdout_copy;
	int ret = 1;

	if (save_stdio(&stdin_copy, &stdout_copy) == -1)
		return (ret);
	if (setup_redirections(data, cmd, &ret) == -1)
	{
		restore_stdio(stdin_copy, stdout_copy);
		return (ret);
	}
	ret = execute_builtin(data, cmd);
	restore_stdio(stdin_copy, stdout_copy);
	return (ret);
}
