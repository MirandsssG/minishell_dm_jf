/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:42:45 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/29 00:53:25 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->infile_fd = -1;
	cmd->args = ft_calloc(100, sizeof(char *));
	return (cmd);
}

int	count_heredocs(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
	{
		if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
			count++;
		i++;
	}
	return (count);
}

static void	handle_token(
	t_cmd *cmd, char **tokens, t_parse_state *st)
{
	if (ft_strcmp(tokens[st->i], "<") == 0 && tokens[st->i + 1])
		cmd->infile = ft_strdup(tokens[++(st->i)]);
	else if (ft_strcmp(tokens[st->i], ">") == 0 && tokens[st->i + 1])
	{
		cmd->outfile = ft_strdup(tokens[++(st->i)]);
		cmd->append = 0;
	}
	else if (ft_strcmp(tokens[st->i], ">>") == 0 && tokens[st->i + 1])
	{
		cmd->outfile = ft_strdup(tokens[++(st->i)]);
		cmd->append = 1;
	}
	else if (ft_strcmp(tokens[st->i], "<<") == 0 && tokens[st->i + 1])
	{
		cmd->heredoc = 1;
		cmd->heredoc_delim[st->h_i++] = ft_strdup(tokens[++(st->i)]);
	}
	else
		cmd->args[st->arg_i++] = remove_quotes(tokens[st->i]);
}

int	parse_redirs_and_args(t_cmd *cmd, char **tokens, int i)
{
	t_parse_state	st;

	st.i = i;
	st.arg_i = 0;
	st.h_i = 0;
	while (tokens[st.i] && ft_strcmp(tokens[st.i], "|") != 0)
	{
		handle_token(cmd, tokens, &st);
		st.i++;
	}
	cmd->args[st.arg_i] = NULL;
	if (cmd->heredoc_delim)
		cmd->heredoc_delim[st.h_i] = NULL;
	return (st.i);
}
