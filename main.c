/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:53:55 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/08 22:50:27 by tafonso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrlc_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	
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
			break;
		}
		if (*data.input)
			add_history(data.input);
		parse_and_exec(&data);
		free(data.input);
		data.input = NULL;
		if (data.should_exit)
			break;
	}
	free(data.input);
	free_tokens(data.tokens);
	if (data.env_list)
		free_env_list(data.env_list);
	return (0);
}
