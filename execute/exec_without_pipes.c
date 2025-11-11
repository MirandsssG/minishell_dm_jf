/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_without_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:05:03 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/11/11 15:38:48 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	if ((!cmd || !cmd->args || !cmd->args[0]) && !cmd->heredoc)
	{
		execute_builtin_with_redirections(data, cmd);
		return ;
	}
	if (cmd->heredoc)
		process_heredocs(cmd);
	envp = env_list_to_envp(data->env_list);
	if (!envp)
		return;
	pid = fork();
	if (pid < 0)
	{
		perror ("fork");
		free_envp(envp);
		return;
	}
	if (pid == 0)
	{
		if (cmd->infile_fd > 0)
		{
			if (dup2(cmd->infile_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 infile_fd");
				close(cmd->infile_fd);
				exit(EXIT_FAILURE);
			}
			close(cmd->infile_fd);
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
		if (cmd->args && cmd->args[0])
		{
			cmd_path = get_cmd_path(cmd->args[0], data->env_list);
			if (!cmd_path)
			{
				perror("command not found");
				free_envp(envp);
				free_tokens(data->tokens);
				exit(127);
			}
			execve(cmd_path, cmd->args, envp);
			perror("execve");
			free(cmd_path);
			free_envp(envp);
			exit(EXIT_FAILURE);
		}
		free_envp(envp);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		if (cmd->heredoc && cmd->infile_fd > 0)
		{
			close(cmd->infile_fd);
			cmd->infile_fd = -1;
		}
		free_envp(envp);
	}
}
