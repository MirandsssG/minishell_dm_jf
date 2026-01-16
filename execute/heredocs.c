/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 12:55:43 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/16 18:35:12 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_child(const char *delimiter, int pipe_fd[2], t_data *data)
{
	char	*line;
	char	*cmp_delim;
	int		expand;

	signal(SIGINT, sig_heredoc);
	close(pipe_fd[0]);
	cmp_delim = get_heredoc_delim(delimiter, &expand);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("warning: heredoc delimited by end-of-file", 2);
			break ;
		}
		if (ft_strcmp(line, cmp_delim) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc(line, pipe_fd[1], expand, data);
	}
	free(cmp_delim);
	close(pipe_fd[1]);
	exit(0);
}

static int	run_heredoc(const char *delimiter, t_data *data)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	void	(*prev_sigint)(int);

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	prev_sigint = signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		signal(SIGINT, prev_sigint);
		return (close(pipe_fd[0]), close(pipe_fd[1]), perror("fork"), -1);
	}
	if (pid == 0)
		heredoc_child(delimiter, pipe_fd, data);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, prev_sigint);
	if (WIFSIGNALED(status)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
		return (close(pipe_fd[0]), -1);
	return (pipe_fd[0]);
}

static int	process_cmd_heredocs(t_cmd *cmd, t_data *data)
{
	int	fd;
	int	tmp;
	int	i;

	fd = -1;
	if (!cmd->heredoc || !cmd->heredoc_delim)
		return (0);
	i = 0;
	while (cmd->heredoc_delim[i])
	{
		tmp = run_heredoc(cmd->heredoc_delim[i], data);
		if (tmp == -1)
		{
			if (fd != -1)
				close(fd);
			cmd->infile_fd = -1;
			return (data->last_exit_status = 130, -1);
		}
		if (fd != -1)
			close(fd);
		fd = tmp;
		i++;
	}
	cmd->infile_fd = fd;
	return (0);
}

int	process_heredocs(t_cmd *cmds, t_data *data)
{
	while (cmds)
	{
		if (process_cmd_heredocs(cmds, data) == -1)
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}
