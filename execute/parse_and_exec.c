/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:13:21 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/10 16:25:17 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute(t_data *data, t_cmd *cmds)
{
	if (!cmds)
		return ;
	if (cmds->args[0][0] == '\0')
	{
		data->last_exit_status = 127;
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(cmds->args[0], 2);
		ft_putstr_fd("': command not found\n", 2);
		return ;
	}
	if (cmds->args && cmds->args[0] && is_builtin(cmds->args[0])
		&& cmds->next == NULL)
	{
		data->last_exit_status = execute_builtin_with_redirections(data, cmds);
		return ;
	}
	if (cmds->next)
		exec_pipes(data, cmds);
	else
		exec_without_pipes(data, cmds);
}

int	check_tokens_and_syntax(t_data *data)
{
	if (!data->tokens || !data->tokens[0])
	{
		free_split(data->tokens);
		data->tokens = NULL;
		return (1);
	}
	if (check_syntax(data->tokens, data))
	{
		free_split(data->tokens);
		data->tokens = NULL;
		return (1);
	}
	return (0);
}

void	parse_and_exec(t_data *data)
{
	t_cmd	*cmds;

	if (has_unclosed_quotes(data->input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return ;
	}
	tokenize_inputs(data);
	if (check_tokens_and_syntax(data))
		return ;
	expand_variables(data);
	remove_quotes_all(data);
	cmds = parse_cmds(data->tokens);
	if (!cmds)
	{
		free_split(data->tokens);
		data->tokens = NULL;
		return ;
	}
	execute(data, cmds);
	free_cmds(cmds);
	free_split(data->tokens);
	data->tokens = NULL;
}
