/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 19:04:53 by tafonso           #+#    #+#             */
/*   Updated: 2026/01/14 19:36:52 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
