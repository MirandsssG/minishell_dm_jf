/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:45:08 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/11 18:40:00 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*remove_quotes(const char *str)
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
	int		i;
	int		arg_i;
	char	*arg;
	
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
			{
				arg = remove_quotes(tokens[i]);
				new_cmd->args[arg_i++] = arg;
			}
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
