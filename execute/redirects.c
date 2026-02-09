/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 19:04:53 by tafonso           #+#    #+#             */
/*   Updated: 2026/02/09 04:05:18 by tafonso          ###   ########.fr       */
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
	int	fd;
	int	i;

	if (!cmd->infile)
		return (0);
	i = 0;
	while (cmd->infile[i])
	{
		fd = open(cmd->infile[i], O_RDONLY);
		if (fd < 0)
			return (perror("open infile"), 1);
		if (!cmd->infile[i + 1] && dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2 infile"), close(fd), 1);
		close(fd);
		i++;
	}
	return (0);
}

int	redirection_outfile(t_cmd *cmd)
{
	int	fd;
	int	flags;
	int	i;

	if (!cmd->outfile)
		return (0);
	i = 0;
	while (cmd->outfile[i])
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(cmd->outfile[i], flags, 0644);
		if (fd < 0)
			return (perror("open outfile"), 1);
		if (!cmd->outfile[i + 1] && dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2 outfile"), close(fd), 1);
		close(fd);
		i++;
	}
	return (0);
}
