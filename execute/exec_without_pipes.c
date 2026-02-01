/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_without_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:05:03 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/01 19:48:22 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_cmd_path(t_cmd *cmd, t_data *data, char **envp,
				char *cmd_path)
{
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_envp(envp);
		free_tokens(data->tokens);
		exit(127);
	}
	if (access(cmd_path, F_OK) != 0)
	{
		perror(cmd->args[0]);
		exit(127);
	}
	if (is_directory(cmd_path))
	{
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		perror(cmd->args[0]);
		exit(126);
	}
}

static void	child_setup_and_exec(t_data *data, t_cmd *cmd, char **envp)
{
	char	*cmd_path;

	if (heredoc_infile(cmd))
		exit(EXIT_FAILURE);
	else if (redirection_infile(cmd))
		exit(EXIT_FAILURE);
	if (redirection_outfile(cmd))
		exit(EXIT_FAILURE);
	if (cmd->args && cmd->args[0])
	{
		cmd_path = find_command_path(cmd->args[0], envp);
		check_cmd_path(cmd, data, envp, cmd_path);
		execve(cmd_path, cmd->args, envp);
		perror("execve");
		free(cmd_path);
		free_envp(envp);
		exit(EXIT_FAILURE);
	}
	free_envp(envp);
	exit(EXIT_SUCCESS);
}

static void	parent_wait_and_cleanup(t_data *data, pid_t pid, t_cmd *cmd,
									char **envp)
{
	int	status;

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
	pid_t	pid;
	char	**envp;

	if (!cmd)
		return ;
	if ((!cmd->args || !cmd->args[0]) && !cmd->heredoc)
	{
		data->last_exit_status = execute_builtin_with_redirections(data, cmd);
		return ;
	}
	if (process_heredocs(cmd, data) == -1)
		return ;
	envp = env_list_to_envp(data->env_list);
	if (!envp)
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror ("fork");
		free_envp(envp);
		return ;
	}
	if (pid == 0)
		child_setup_and_exec(data, cmd, envp);
	parent_wait_and_cleanup(data, pid, cmd, envp);
}
