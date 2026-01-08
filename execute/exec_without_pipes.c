/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_without_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:05:03 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/08 22:45:13 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static char	*get_env_var(char *key, t_env *env_list)
// {
// 	while (env_list)
// 	{
// 		if (ft_strcmp(env_list->key, key) == 0)
// 			return (env_list->value);
// 		env_list = env_list->next;
// 	}
// 	return (NULL);
// }

// static char	*get_cmd_path(char *cmd, t_env *env_list)
// {
// 	char	**paths;
// 	char	*full_path;
// 	char	*path;
// 	char	*tmp;
// 	int		i;

// 	if (ft_strchr(cmd, '/'))
// 		return (ft_strdup(cmd));
// 	path = get_env_var("PATH", env_list);
// 	if (!path)
// 		return (NULL);
// 	paths = ft_split(path, ':');
// 	if (!paths)
// 		return (NULL);
// 	i = 0;
// 	while (paths[i])
// 	{
// 		tmp = ft_strjoin(paths[i], "/");
// 		full_path = ft_strjoin_free(tmp, cmd);
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			free_split(paths);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	free_split(paths);
// 	return (NULL);
// }

static void child_setup_and_exec(t_data *data, t_cmd *cmd, char **envp)
{
	int infile_fd;
	
	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
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
		int flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		int outfile_fd = open(cmd->outfile, flags, 0644);
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
		char *cmd_path = find_command_path(cmd->args[0], envp);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: command not found\n", cmd->args[0]);
			free_envp(envp);
			free_tokens(data->tokens);
			exit(127);
		}
		if (access(cmd_path, F_OK) != 0)
		{
			perror(cmd->args[0]);
			exit(127);
		}
		if (access(cmd_path, X_OK) != 0)
		{
			perror(cmd->args[0]);
			exit(126);
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

static void parent_wait_and_cleanup(t_data *data, pid_t pid, t_cmd *cmd,
									char **envp)
{
	int status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	signal(SIGINT, ctrlc_handler);
	if (cmd->heredoc && cmd->infile_fd > 0)
	{
		close(cmd->infile_fd);
		cmd->infile_fd = -1;
	}
	free_envp(envp);
}

void	exec_without_pipes(t_data *data, t_cmd *cmd)
{
	pid_t pid;
	char **envp;

	if (!cmd)
		return;
	if ((!cmd->args || !cmd->args[0]) && !cmd->heredoc)
	{
		execute_builtin_with_redirections(data, cmd);
		return;
	}
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
		child_setup_and_exec(data, cmd, envp);
	else
		parent_wait_and_cleanup(data, pid, cmd, envp);
}
