/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 12:55:43 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/11/11 14:32:08 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_heredoc(const char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("warning: heredoc delimited by end-of-file", 2);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		} 
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	process_heredocs(t_cmd *cmds)
{
	int	fd;
	int	i;
	int	tmp_fd;
	
	while (cmds)
	{
		fd = -1;
		if (cmds->heredoc && cmds->heredoc_delim)
		{
			i = 0;
			while (cmds->heredoc_delim[i])
			{
				tmp_fd = handle_heredoc(cmds->heredoc_delim[i]);
				if (fd != -1)
					close(fd);
				fd = tmp_fd;
				i++;
			}
			cmds->infile_fd = fd;
		}
		else
			cmds->infile_fd = -1;
		cmds = cmds->next;
	}
}
