/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:06:28 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/12 01:26:22 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->heredoc_delim)
		{
			i = 0;
			while (cmd->heredoc_delim[i])
				free(cmd->heredoc_delim[i++]);
			free(cmd->heredoc_delim);
		}
		if (cmd->infile_fd > 0)
		{
			close(cmd->infile_fd);
			cmd->infile_fd = -1;
		}
		free(cmd);
		cmd = tmp;
	}	
}
