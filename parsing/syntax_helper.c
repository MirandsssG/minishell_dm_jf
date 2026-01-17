/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:43:05 by tafonso           #+#    #+#             */
/*   Updated: 2026/01/17 17:43:15 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_pipe(const char *t)
{
	return (ft_strcmp(t, "|") == 0);
}

int	is_redirection(const char *t)
{
	return (!ft_strcmp(t, "<")
		|| !ft_strcmp(t, ">")
		|| !ft_strcmp(t, ">>")
		|| !ft_strcmp(t, "<<"));
}

int	is_operator(const char *t)
{
	return (is_pipe(t) || is_redirection(t));
}

int	is_unsupported(const char *t)
{
	return (!ft_strcmp(t, "||")
		|| !ft_strcmp(t, "&&")
		|| !ft_strcmp(t, ";"));
}
