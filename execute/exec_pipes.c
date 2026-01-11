/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:14:59 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/09 21:09:55 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_cmd *cmd, t_data *data, int *prev_fd, pid_t pid, int *fd)
{
	char **envp;
	char *cmd_path;
	
	if (pid == 0)
	{
		// signal(SIGINT, SIG_DFL);
    	// signal(SIGQUIT, SIG_DFL);
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
		heredoc_infile(cmd);
		if (redirection_infile(cmd) == -1)
			exit(EXIT_FAILURE);
		if (redirection_outfile(cmd) == -1)
			exit(EXIT_FAILURE);
		if ((!cmd->args || !cmd->args[0]) && !cmd->heredoc)
			exit(execute_builtin(data, cmd));
		if (cmd->args && cmd->args[0])
		{
			if (is_builtin(cmd->args[0]))
				exit(execute_builtin(data, cmd));
			else
			{
				envp = env_list_to_envp(data->env_list);
				cmd_path = find_command_path(cmd->args[0], envp);
				if (!cmd_path)
				{
					fprintf(stderr, "%s: command not found\n", cmd->args[0]);
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
			exit(EXIT_SUCCESS);
	}
}

void	exec_pipes(t_data *data, t_cmd *cmds)
{
	int     prev_fd;
	int     fd[2];
	pid_t   pid;
	int     status;
	t_cmd   *cmd;
	int     children;

	if (!cmds)
		return ;
	prev_fd = -1;
	children = 0;
	cmd = cmds;
	process_heredocs(cmd);
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				if (prev_fd != -1)
					close(prev_fd);
				return;
			}
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->next)
			{
				close(fd[0]);
				close(fd[1]);
			}
			return ;
		}
		child_process(cmd, data, &prev_fd, pid, fd);
		children++;
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		else
			prev_fd = -1;
		cmd = cmd->next;
	}
	while (children-- > 0)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, ctrlc_handler);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
	}
}
