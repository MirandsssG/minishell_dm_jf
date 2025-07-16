/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_without_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:05:03 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/16 11:00:34 by mirandsssg       ###   ########.fr       */
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
		if (!line || ft_strcmp(line, delimiter) == 0)
			break;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

static char	*get_env_var(char *key, t_env *env_list)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

static char	*get_cmd_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	char	*path;
	char	*tmp;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_env_var("PATH", env_list);
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

void	exec_without_pipes(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	int		infile_fd;
	int		outfile_fd;
	int		flags;
	char	**envp;
	char	*cmd_path;
	int		heredoc_fd;
	
	infile_fd = -1;
	outfile_fd = -1;
	pid = fork();
	if (pid == 0)
	{
		if (cmd->heredoc)
		{
			heredoc_fd = handle_heredoc(cmd->heredoc_delim);
			if (heredoc_fd < 0)
				exit(EXIT_FAILURE);
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		else if (cmd->infile)
		{
			infile_fd = open(cmd->infile, O_RDONLY);
			if (infile_fd < 0)
			{
				perror("open infile");
				exit(EXIT_FAILURE);
			}
			dup2(infile_fd, STDIN_FILENO);
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
				exit(EXIT_FAILURE);
			}
			dup2(outfile_fd, STDOUT_FILENO);
			close(outfile_fd);
		}
		envp = env_list_to_envp(data->env_list);
		if (!envp)
			exit(EXIT_FAILURE);
		cmd_path = get_cmd_path(cmd->args[0], data->env_list);
		if (!cmd_path)
		{
			perror("command not found");
			free_envp(envp);
			exit(127);
		}
		if (execve(cmd_path, cmd->args, envp) == -1)
		{
			perror("execve");
			free(cmd_path);
			free_envp(envp);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
	}
}
