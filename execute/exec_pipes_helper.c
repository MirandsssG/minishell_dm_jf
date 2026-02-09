/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:42:33 by tafonso           #+#    #+#             */
/*   Updated: 2026/02/09 00:17:57 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parent_fds(t_cmd *cmd, int *prev_fd, int *fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	else
		*prev_fd = -1;
}

void	wait_children(int children, t_data *data, pid_t pid)
{
	int	status;

	while (children-- > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, ctrlc_handler);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
	}
}

int	pipe_error(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	perror("pipe");
	return (-1);
}

int	fork_error(int prev_fd, int *fd, int has_pipe)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (has_pipe)
	{
		close(fd[0]);
		close(fd[1]);
	}
	perror("fork");
	return (-1);
}
