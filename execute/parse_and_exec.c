/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogo <diogo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:13:21 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/18 01:11:55 by diogo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static void print_cmds(t_cmd *cmds)
// {
//     t_cmd *tmp = cmds;
//     int cmd_index = 0;

//     while (tmp)
//     {
//         printf("Command %d:\n", cmd_index);
//         printf("  args: ");
//         if (tmp->args)
//         {
//             for (int i = 0; tmp->args[i]; i++)
//                 printf("'%s' ", tmp->args[i]);
//         }
//         else
//             printf("(null)");

//         printf("\n");
//         printf("  infile: %s\n", tmp->infile ? tmp->infile : "NULL");
//         printf("  outfile: %s\n", tmp->outfile ? tmp->outfile : "NULL");
//         printf("  append: %d\n", tmp->append);
//         printf("  heredoc: %d\n", tmp->heredoc);
//         // printf("  heredoc_delim: %s\n", tmp->heredoc_delim ? tmp->heredoc_delim : "NULL");
//         printf("\n");

//         tmp = tmp->next;
//         cmd_index++;
//     }
// }

void	execute(t_data *data, t_cmd *cmds)
{
	if (cmds && cmds->args && is_builtin(cmds->args[0]) && cmds->next == NULL)
	{
		data->last_exit_status = execute_builtin_with_redirections(data, cmds);
		return ;
	}
	if (cmds->next)
	{
		// printf("exec with pipes\n");
		exec_pipes(data, cmds);
	}
	else
	{
		// printf("exec without pipes\n");
		exec_without_pipes(data, cmds);
	}
}

void	parse_and_exec(t_data *data)
{
	t_cmd	*cmds;

	if (has_unclosed_quotes(data->input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
		return ;
	}
	tokenize_inputs(data);
	if (!data->tokens || !data->tokens[0])
	{
		free_split(data->tokens);
		data->tokens = NULL;
		return ;
	}
	if (check_syntax(data->tokens, data))
	{
		free_split(data->tokens);
		data->tokens = NULL;
		return ;
	}
	for (int i = 0; data->tokens[i]; i++)
		printf("token[%d] = '%s'\n", i, data->tokens[i]);
	expand_variables(data);
	for (int i = 0; data->tokens[i]; i++)
    	printf("token_expanded[%d] = '%s'\n", i, data->tokens[i]);
	cmds = parse_cmds(data->tokens);
	if (!cmds)
	{
		free_split(data->tokens);
		data->tokens = NULL;
		return ;
	}
	// print_cmds(cmds);
	execute(data, cmds);
	free_cmds(cmds);
	free_split(data->tokens);
	data->tokens = NULL;
}
