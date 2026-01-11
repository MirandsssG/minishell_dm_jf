/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_before_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 00:26:29 by tafonso           #+#    #+#             */
/*   Updated: 2026/01/10 00:26:29 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(char *path)
{
	struct stat st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}


