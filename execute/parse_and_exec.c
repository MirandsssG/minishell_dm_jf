/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:13:21 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/06/22 14:19:50 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_and_exec(t_data *data)
{
	tokenize_inputs(data);
	for (int i = 0; data->tokens[i]; i++)
        printf("token[%d] = '%s'\n", i, data->tokens[i]);
	expand_variables(data);
	for (int i = 0; data->tokens[i]; i++)
        printf("token_expanded[%d] = '%s'\n", i, data->tokens[i]);
	if (is_builtin(data->tokens[0]))
		execute_builtin(data);
	else
		printf("is not Builtin\n");
}
