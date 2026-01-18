/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:42:33 by tafonso           #+#    #+#             */
/*   Updated: 2026/01/18 15:34:27 by tafonso          ###   ########.fr       */
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

void	wait_children(int children, t_data *data)
{
	int	status;

	while (children-- > 0)
	{
		signal(SIGINT, SIG_IGN);
		wait(&status);
		signal(SIGINT, ctrlc_handler);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->last_exit_status = 128 + WTERMSIG(status);
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
