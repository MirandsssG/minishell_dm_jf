/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 03:52:46 by tafonso           #+#    #+#             */
/*   Updated: 2026/01/14 19:52:06 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ctrlc_handler(int sig)
{
	(void)sig;
	g_exit_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
	exit(130);
}

int	sig_ctrl(t_data *data)
{
	if (g_exit_signal)
	{
		data->last_exit_status = g_exit_signal;
		g_exit_signal = 0;
		if (data->input == NULL || data->input[0] == '\0')
		{
			free(data->input);
			data->input = NULL;
			return (1);
		}
	}
	return (0);
}
