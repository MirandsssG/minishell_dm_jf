/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:45:08 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/09 00:02:45 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_infiles(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
	{
		if (ft_strcmp(tokens[i], "<") == 0)
			count++;
		i++;
	}
	return (count);
}

int	count_outfiles(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
	{
		if (ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0)
			count++;
		i++;
	}
	return (count);
}

char	*remove_quotes(const char *str)
{
	char	*new_str;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

t_cmd *parse_cmds(char **tokens)
{
    t_cmd *head;
    t_cmd *curr;
    t_cmd *new;
    int i;
    int arg_count;
    int in_count;
    int out_count;
    int hd_count;

    head = NULL;
    curr = NULL;
    i = 0;

    while (tokens[i])
    {
        arg_count = count_args(tokens, i);
        in_count = count_infiles(tokens, i);
        out_count = count_outfiles(tokens, i);
        hd_count = count_heredocs(tokens, i);
        new = init_cmd(arg_count, in_count, out_count, hd_count);
        if (!new)
            return (NULL);
        i = parse_redirs_and_args(new, tokens, i);
        if (!head)
            head = new;
        else
            curr->next = new;
        curr = new;
        if (tokens[i] && ft_strcmp(tokens[i], "|") == 0)
            i++;
    }
    return head;
}
