/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:45:08 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/11 00:13:28 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_cmds(char **tokens)
{
	int	i;
	int	arg_i;
	
	t_cmd *head = NULL;
	t_cmd *current = NULL;
	i = 0;
	while (tokens[i])
	{
		t_cmd *new_cmd = ft_calloc(1, sizeof(t_cmd));
		new_cmd->args = ft_calloc(100, sizeof(char*));
		arg_i = 0;
		while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
		{
			if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
				new_cmd->infile = ft_strdup(tokens[++i]);
			else if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
			{
				new_cmd->outfile = ft_strdup(tokens[++i]);
				new_cmd->append = 0;
			}
			else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
			{
				new_cmd->outfile = ft_strdup(tokens[++i]);
				new_cmd->append = 1;
			}
			else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
			{
				new_cmd->heredoc = 1;
				new_cmd->heredoc_delim = ft_strdup(tokens[++i]);
			}
			else
				new_cmd->args[arg_i++] = ft_strdup(tokens[i]);
			i++;
		}
		new_cmd->args[arg_i] = NULL;
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		if (tokens[i] && ft_strcmp(tokens[i], "|") == 0)
			i++;
	}
	return (head);
}
