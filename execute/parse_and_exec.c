/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:13:21 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/02/09 00:23:07 by mirandsssg       ###   ########.fr       */
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

//         // Print args
//         printf("  args: ");
//         if (tmp->args)
//         {
//             for (int i = 0; tmp->args[i]; i++)
//                 printf("'%s' ", tmp->args[i]);
//         }
//         else
//             printf("(null)");
//         printf("\n");

//         // Print infile array
//         printf("  infile: ");
//         if (tmp->infile)
//         {
//             for (int i = 0; tmp->infile[i]; i++)
//                 printf("'%s' ", tmp->infile[i]);
//         }
//         else
//             printf("NULL");
//         printf("\n");

//         // Print outfile array
//         printf("  outfile: ");
//         if (tmp->outfile)
//         {
//             for (int i = 0; tmp->outfile[i]; i++)
//                 printf("'%s' ", tmp->outfile[i]);
//         }
//         else
//             printf("NULL");
//         printf("\n");

//         printf("  append: %d\n", tmp->append);
//         printf("  heredoc: %d\n", tmp->heredoc);

//         // Print heredoc delimiters
//         printf("  heredoc_delim: ");
//         if (tmp->heredoc_delim)
//         {
//             for (int i = 0; tmp->heredoc_delim[i]; i++)
//                 printf("'%s' ", tmp->heredoc_delim[i]);
//         }
//         else
//             printf("NULL");
//         printf("\n\n");

//         tmp = tmp->next;
//         cmd_index++;
//     }
// }

void	execute(t_data *data, t_cmd *cmds)
{
	if (cmds->args[0][0] == '\0' && !cmds->args[1])
	{
		data->last_exit_status = 0;
		return ;
	}
	if (cmds && cmds->args && is_builtin(cmds->args[0]) && cmds->next == NULL)
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
	// for (int i = 0; data->tokens[i]; i++)
	// 	printf("token_expanded[%d] = '%s'\n", i, data->tokens[i]);
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
