/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds_helpers_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 23:29:40 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/10 17:23:08 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	alloc_redirs(t_cmd *cmd, char **tokens, int i)
{
	int	in_count;
	int	out_count;
	int	hd_count;

	in_count = count_infiles(tokens, i);
	out_count = count_outfiles(tokens, i);
	hd_count = count_heredocs(tokens, i);
	if (in_count > 0)
		cmd->infile = ft_calloc(in_count + 1, sizeof(char *));
	if (out_count > 0)
		cmd->outfile = ft_calloc(out_count + 1, sizeof(char *));
	if (hd_count > 0)
		cmd->heredoc_delim = ft_calloc(hd_count + 1, sizeof(char *));
}

int	count_args(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
	{
		if (is_redirect(tokens[i]))
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

void	add_arg(t_cmd *cmd, char *token)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		i++;
	cmd->args[i] = ft_strdup(token);
}

int	is_redirect(char *s)
{
	return (!ft_strcmp(s, "<")
		|| !ft_strcmp(s, ">")
		|| !ft_strcmp(s, ">>")
		|| !ft_strcmp(s, "<<"));
}
