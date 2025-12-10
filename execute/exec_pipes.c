/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:14:59 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/12/10 22:51:15 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Simple pipeline executor.
 * For each command in the list:
 *  - create a pipe for the next command (if any)
 *  - fork a child
 *  - in the child: connect prev pipe read end to STDIN, next pipe write end to STDOUT
 *    apply infile/heredoc/outfile redirections, then execute builtin or external
 *  - in the parent: close unused fds and continue
 * At the end, wait for all children and set data->last_exit_status to the last child's status.
 */

static char *get_cmd_path(char *cmd, t_env *env_list)
{
	char    **paths;
	char    *full_path;
	char    *path;
	char    *tmp;
	int     i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_env_value("PATH", env_list);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(tmp, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
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
		return;
	/* ensure heredocs are processed and cmd->infile_fd set */
	process_heredocs(cmds);

	prev_fd = -1;
	children = 0;
	cmd = cmds;
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
		if (pid == 0)
		{
			/* child */
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					perror("dup2 prev_fd");
				close(prev_fd);
			}
			if (cmd->next)
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					perror("dup2 pipe write");
				close(fd[0]);
				close(fd[1]);
			}
			/* handle heredoc / infile_fd */
			if (cmd->infile_fd > 0)
			{
				if (dup2(cmd->infile_fd, STDIN_FILENO) == -1)
					perror("dup2 infile_fd");
				close(cmd->infile_fd);
			}
			else if (cmd->infile)
			{
				int infile_fd = open(cmd->infile, O_RDONLY);
				if (infile_fd < 0)
				{
					perror("open infile");
					exit(1);
				}
				if (dup2(infile_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 infile");
					close(infile_fd);
					exit(1);
				}
				close(infile_fd);
			}
			/* handle outfile */
			if (cmd->outfile)
			{
				int flags = O_WRONLY | O_CREAT;
				if (cmd->append)
					flags |= O_APPEND;
				else
					flags |= O_TRUNC;
				int outfile_fd = open(cmd->outfile, flags, 0644);
				if (outfile_fd < 0)
				{
					perror("open outfile");
					exit(1);
				}
				if (dup2(outfile_fd, STDOUT_FILENO) == -1)
				{
					perror("dup2 outfile");
					close(outfile_fd);
					exit(1);
				}
				close(outfile_fd);
			}

			/* execute builtin or external */
			if ((!cmd->args || !cmd->args[0]) && !cmd->heredoc)
				exit(execute_builtin(data, cmd));
			if (cmd->args && cmd->args[0])
			{
				if (is_builtin(cmd->args[0]))
					exit(execute_builtin(data, cmd));
				else
				{
					char **envp = env_list_to_envp(data->env_list);
					char *cmd_path = get_cmd_path(cmd->args[0], data->env_list);
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
		/* parent */
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
	/* wait for children */
	while (children-- > 0)
	{
		if (wait(&status) == -1)
			break;
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
	}
}
