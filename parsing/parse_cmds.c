/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:45:08 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/29 00:47:11 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_cmd	*parse_cmds(char **tokens)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_cmd	*new;
	int		i;
	int		hd_count;

	head = NULL;
	curr = NULL;
	i = 0;
	while (tokens[i])
	{
		new = init_cmd();
		hd_count = count_heredocs(tokens, i);
		if (hd_count > 0)
			new->heredoc_delim = ft_calloc(hd_count + 1, sizeof(char *));
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
