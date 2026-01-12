/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:22:14 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/12 04:56:05 by tafonso          ###   ########.fr       */
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

int	heredoc_infile(t_cmd *cmd)
{
	if (cmd->infile_fd > 0)
	{
		if (dup2(cmd->infile_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 infile");
			close (cmd->infile_fd);
			return (1);
		}
		close(cmd->infile_fd);
	}
	return (0);
}

int	redirection_infile(t_cmd *cmd)
{
	int		infile_fd;

	infile_fd = -1;
	if (cmd->infile)
	{
		infile_fd = open(cmd->infile, O_RDONLY);
		if (infile_fd < 0)
		{
			perror("open infile");
			return (1);
		}
		if (dup2(infile_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 infile");
			close (infile_fd);
			return (1);
		}
		close(infile_fd);
	}
	return (0);
}

int	redirection_outfile(t_cmd *cmd)
{
	int		outfile_fd;
	int		flags;

	outfile_fd = -1;
	if (cmd->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		outfile_fd = open(cmd->outfile, flags, 0644);
		if (outfile_fd < 0)
			return (perror("open outfile"), 1);
		if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 outfile");
			close(outfile_fd);
			return (1);
		}
		close(outfile_fd);
	}
	return (0);
}
int	execute_builtin_with_redirections(t_data *data, t_cmd *cmd)
{
	int		stdin_copy;
	int		stdout_copy;
	int		ret;
	
	ret = 1;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (process_heredocs(cmd, data) == -1)
	{
		ret = data->last_exit_status;
		goto restore;
	}
	if (stdin_copy == -1 || stdout_copy == -1)
		return (perror("dup"),	close(stdin_copy), close(stdout_copy), ret);
	if (heredoc_infile(cmd) == -1)
		goto restore;
	if (redirection_infile(cmd) == -1)
		goto restore;
	if (redirection_outfile(cmd) == -1)
		goto restore;
	ret = execute_builtin(data, cmd);
	goto restore;
restore:
	if (dup2(stdin_copy, STDIN_FILENO) == -1)
		perror("dup2 restore stdin");
	if (dup2(stdout_copy, STDOUT_FILENO) == -1)
		perror("dup2 restore stdout");
	close(stdin_copy);
	close(stdout_copy);
	return (ret);
}
