/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsilveir <jsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:22:14 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/10/22 13:09:35 by jsilveir         ###   ########.fr       */
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
		return (exit_builtin(data));
	return (0);
}

int	execute_builtin_with_redirections(t_data *data, t_cmd *cmd)
{
	int		infile_fd;
	int		outfile_fd;
	int		flags;
	int		stdin_copy;
	int		stdout_copy;
	int		ret;
	
	infile_fd = -1;
	outfile_fd = -1;
	ret = -1;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (stdin_copy == -1 || stdout_copy == -1)
	{
		perror("dup");
		return (-1);
	}
	if (cmd->infile)
	{
		infile_fd = open(cmd->infile, O_RDONLY);
		if (infile_fd < 0)
		{
			perror("open infile");
			goto restore;
		}
		if (dup2(infile_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 infile");
			close (infile_fd);
			goto restore;
		}
		close(infile_fd);
	}
	if (cmd->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		outfile_fd = open(cmd->outfile, flags, 0644);
		if (outfile_fd < 0)
		{
			perror("open outfile");
			goto restore;
		}
		if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 outfile");
			close(outfile_fd);
			goto restore;
		}
		close(outfile_fd);
	}
	ret = execute_builtin(data, cmd);

restore:
	if (dup2(stdin_copy, STDIN_FILENO) == -1)
		perror("dup2 restore stdin");
	if (dup2(stdout_copy, STDOUT_FILENO) == -1)
		perror("dup2 restore stdout");
	close(stdin_copy);
	close(stdout_copy);
	return (ret);
}
