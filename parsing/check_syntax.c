/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:10:31 by dluis-ma          #+#    #+#             */
/*   Updated: 2026/01/17 17:45:10 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*pipe_error_token(char **t, int i)
{
	if (!t[i + 1])
		return ("newline");
	return ("|");
}

static int	syntax_error(char *token, t_data *data)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	data->last_exit_status = 2;
	return (1);
}

int	check_syntax(char **tokens, t_data *data)
{
	int	i;

	i = 0;
	if (!tokens || !tokens[0])
		return (0);
	if (is_pipe(tokens[0]))
		return (syntax_error("|", data));
	while (tokens[i])
	{
		if (is_pipe(tokens[i]) && (!tokens[i + 1]
				|| is_pipe(tokens[i + 1])))
			return (syntax_error(pipe_error_token(tokens, i), data));
		if (is_redirection(tokens[i]))
		{
			if (!tokens[i + 1])
				return (syntax_error("newline", data));
			if (is_operator(tokens[i + 1]))
				return (syntax_error(tokens[i + 1], data));
		}
		if (is_unsupported(tokens[i]))
			return (syntax_error(tokens[i], data));
		i++;
	}
	return (0);
}

