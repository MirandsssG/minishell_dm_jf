/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:53:55 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/24 19:49:48 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ctrlc_handler(int sig)
{
    (void)sig;
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_on_new_line();
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
		if (data.should_exit)
			break;
		free(data.input);
		data.input = NULL;
	}
	free(data.input);
	free_tokens(data.tokens);
	if (data.env_list)
		free_env_list(data.env_list);
	return (0);
}
