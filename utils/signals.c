/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 03:52:46 by tafonso           #+#    #+#             */
/*   Updated: 2026/01/11 23:59:25 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ctrlc_handler(int sig)
{
	(void)sig;
	exit_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void sig_heredoc(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    close(STDIN_FILENO);
	exit(130);
}