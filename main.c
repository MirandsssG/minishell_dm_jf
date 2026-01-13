/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:53:55 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/12 21:47:24 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_signal = 0;

static int	sig_ctrl(t_data *data)
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

static void	cleanup(t_data *data)
{
	free(data->input);
	free_tokens(data->tokens);
	if (data->env_list)
		free_env_list(data->env_list);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(t_data));
	data.env_list = init_env(envp);
	signal(SIGINT, ctrlc_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		data.input = readline("minishell$ ");
		if (!data.input)
		{
			printf("exit\n");
			break ;
		}
		if (sig_ctrl(&data))
			continue ;
		if (*data.input)
			add_history(data.input);
		parse_and_exec(&data);
		free(data.input);
		data.input = NULL;
		if (data.should_exit)
			break ;
	}
	return (cleanup(&data), data.last_exit_status);
}
