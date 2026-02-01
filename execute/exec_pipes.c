/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:14:59 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/01 18:45:43 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_child_pipes(t_cmd *cmd, int *prev_fd, int *fd)
{
	if (*prev_fd != -1)
	{
		if (dup2(*prev_fd, STDIN_FILENO) == -1)
			perror("dup2 prev_fd");
		close(*prev_fd);
	}
	if (cmd->next)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			perror("dup2 pipe write");
		close(fd[0]);
		close(fd[1]);
	}
}

static void	handle_redirections(t_cmd *cmd)
{
	if (heredoc_infile(cmd))
		exit(EXIT_FAILURE);
	if (redirection_infile(cmd))
		exit(EXIT_FAILURE);
	if (redirection_outfile(cmd))
		exit(EXIT_FAILURE);
}

static void	exec_command(t_cmd *cmd, t_data *data)
{
	char	**envp;
	char	*cmd_path;

	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(data, cmd));
	else
	{
		envp = env_list_to_envp(data->env_list);
		cmd_path = find_command_path(cmd->args[0], envp);
		if (!cmd_path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putendl_fd(": command not found", 2);
			free_envp(envp);
			exit(127);
		}
		execve(cmd_path, cmd->args, envp);
		perror("execve");
		free(cmd_path);
		free_envp(envp);
		exit(EXIT_FAILURE);
	}
}

void	child_process(t_cmd *cmd, t_data *data, int *prev_fd, int *fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(cmd, prev_fd, fd);
	handle_redirections(cmd);
	if (cmd->args && cmd->args[0])
		exec_command(cmd, data);
	exit(EXIT_SUCCESS);
}

void	exec_pipes(t_data *data, t_cmd *cmds)
{
	int		prev_fd;
	int		fd[2];
	pid_t	pid;
	int		children;

	prev_fd = -1;
	children = 0;
	if (!cmds || process_heredocs(cmds, data) == -1)
		return ;
	while (cmds)
	{
		if (cmds->next && pipe(fd) == -1)
			return ((void)pipe_error(prev_fd));
		pid = fork();
		if (pid < 0)
			return ((void)fork_error(prev_fd, fd, cmds->next != NULL));
		if (pid == 0)
			child_process(cmds, data, &prev_fd, fd);
		children++;
		parent_fds(cmds, &prev_fd, fd);
		cmds = cmds->next;
	}
	wait_children(children, data, pid);
}
