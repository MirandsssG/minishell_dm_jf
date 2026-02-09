/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:42:45 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/09 00:21:16 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_cmd(int arg_count, int in_count, int out_count, int hd_count)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->infile_fd = -1;
	cmd->append = 0;
	cmd->heredoc = 0;
	if (arg_count > 0)
		cmd->args = ft_calloc(arg_count + 1, sizeof(char *));
	if (in_count > 0)
		cmd->infile = ft_calloc(in_count + 1, sizeof(char *));
	if (out_count > 0)
		cmd->outfile = ft_calloc(out_count + 1, sizeof(char *));
	if (hd_count > 0)
		cmd->heredoc_delim = ft_calloc(hd_count + 1, sizeof(char *));
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

static void	handle_token(t_cmd *cmd, char **tokens, t_parse_state *st)
{
	if (!tokens[st->i])
		return ;
	if (ft_strcmp(tokens[st->i], "<") == 0 && tokens[st->i + 1])
		cmd->infile[st->in_i++] = ft_strdup(tokens[++st->i]);
	else if (ft_strcmp(tokens[st->i], ">") == 0 && tokens[st->i + 1])
	{
		cmd->outfile[st->out_i++] = ft_strdup(tokens[++st->i]);
		cmd->append = 0;
	}
	else if (ft_strcmp(tokens[st->i], ">>") == 0 && tokens[st->i + 1])
	{
		cmd->outfile[st->out_i++] = ft_strdup(tokens[++st->i]);
		cmd->append = 1;
	}
	else if (ft_strcmp(tokens[st->i], "<<") == 0 && tokens[st->i + 1])
	{
		cmd->heredoc_delim[st->h_i++] = ft_strdup(tokens[++st->i]);
		cmd->heredoc = 1;
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
	st.in_i = 0;
	st.out_i = 0;
	while (tokens[st.i] && ft_strcmp(tokens[st.i], "|") != 0)
	{
		handle_token(cmd, tokens, &st);
		st.i++;
	}
	if (cmd->args)
		cmd->args[st.arg_i] = NULL;
	if (cmd->infile)
		cmd->infile[st.in_i] = NULL;
	if (cmd->outfile)
		cmd->outfile[st.out_i] = NULL;
	if (cmd->heredoc_delim)
		cmd->heredoc_delim[st.h_i] = NULL;
	return (st.i);
}
