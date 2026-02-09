/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:45:08 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/09 00:14:49 by mirandsssg       ###   ########.fr       */
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

static t_cmd	*create_cmd(char **tokens, int i)
{
	return (init_cmd(count_args(tokens, i),
			count_infiles(tokens, i),
			count_outfiles(tokens, i),
			count_heredocs(tokens, i)));
}

t_cmd	*parse_cmds(char **tokens)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_cmd	*new;
	int		i;

	head = NULL;
	curr = NULL;
	i = 0;
	while (tokens[i])
	{
		new = create_cmd(tokens, i);
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
	return (head);
}
