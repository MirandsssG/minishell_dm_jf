/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:06:28 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/09 03:36:16 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_redirections(t_cmd *cmd)
{
	int	i;

	if (cmd->infile)
	{
		i = 0;
		while (cmd->infile[i])
			free(cmd->infile[i++]);
		free(cmd->infile);
	}
	if (cmd->outfile)
	{
		i = 0;
		while (cmd->outfile[i])
			free(cmd->outfile[i++]);
		free(cmd->outfile);
	}
	if (cmd->heredoc_delim)
	{
		i = 0;
		while (cmd->heredoc_delim[i])
			free(cmd->heredoc_delim[i++]);
		free(cmd->heredoc_delim);
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		free_redirections(cmd);
		if (cmd->infile_fd > 0)
		{
			close(cmd->infile_fd);
			cmd->infile_fd = -1;
		}
		free(cmd);
		cmd = tmp;
	}
}
