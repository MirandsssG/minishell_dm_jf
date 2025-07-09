/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:56:59 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/09 20:29:39 by mirandsssg       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "includes/libft/libft.h"

typedef struct s_env
{
	char		*key;
	char		*value;
	struct	s_env *next;
} t_env;

typedef	struct s_data
{
	char		*input;
	char		**tokens;
	size_t		i;
	size_t		j;
	size_t		k;
	int			var_start;
	char		*var_name;
	size_t		var_i;
	t_env		*env_list;
	int			should_exit;
}	t_data;



// Builtins

int		execute_builtin(t_data *data);

int		cd_builtin(t_data *data);

int		echo_builtin(t_data *data);

int		env_builtin(t_data *data);

int		exit_builtin(t_data *data);

void	free_tokens(char **tokens);

void	free_env_list(t_env *env);

int		export_builtin(t_data *data);

int		is_valid_varname(char *str);

int		pwd_builtin(void);

int		unset_builtin(t_data *data);

// Env

t_env	*init_env(char **envp);

t_env	*new_env_node(char *key, char *value);

char	*get_value(char *env_str);

char	*get_key(char *env_str);

// Execute

void	parse_and_exec(t_data *data);

// Parsing

void	tokenize_inputs(t_data *data);

void	expand_variables(t_data *data);

int		is_builtin(const char *cmd);

// Utils

char	*ft_strndup(const char *str, size_t n);

int		ft_strcmp(const char *s1, const char *s2);

#endif