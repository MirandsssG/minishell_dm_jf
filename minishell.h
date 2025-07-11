/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirandsssg <mirandsssg@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:56:59 by mirandsssg        #+#    #+#             */
/*   Updated: 2025/07/11 19:13:21 by mirandsssg       ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>

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
	size_t		k;
	size_t		j;
	int			var_start;
	char		*var_name;
	size_t		var_i;
	t_env		*env_list;
	int			should_exit;
	int			last_exit_status;
	int			i;
	int			in_single;
	int			in_double;
}	t_data;

typedef struct s_cmd
{
	char	**args;
	char	*infile;
	char	*outfile;
	int		append;
	int		heredoc;
	char	*heredoc_delim;
	struct	s_cmd *next;
} t_cmd;



// Builtins

int		execute_builtin(t_data *data, t_cmd *cmd);

void	execute_builtin_with_redirections(t_data *data, t_cmd *cmd);

int		cd_builtin(t_data *data, t_cmd *cmd);

int		echo_builtin(t_cmd *cmd);

int		env_builtin(t_data *data);

int		exit_builtin(t_data *data);

void	free_tokens(char **tokens);

void	free_env_list(t_env *env);

int		export_builtin(t_data *data, t_cmd *cmd);

int		is_valid_varname(char *str);

int		pwd_builtin(void);

int		unset_builtin(t_data *data, t_cmd *cmd);

// Env

t_env	*init_env(char **envp);

t_env	*new_env_node(char *key, char *value);

char	*get_value(char *env_str);

char	*get_key(char *env_str);

char	**env_list_to_envp(t_env *env_list);

void	free_envp(char **envp);

// Execute

void	parse_and_exec(t_data *data);

void	execute(t_data *data, t_cmd *cmds);

void	exec_pipes(t_data *data, t_cmd *cmds);

void	exec_without_pipes(t_data *data, t_cmd *cmds);

void	free_cmds(t_cmd *cmd);

// Parsing

void	tokenize_inputs(t_data *data);

void	expand_variables(t_data *data);

int		is_builtin(const char *cmd);

t_cmd	*parse_cmds(char **tokens);

int		has_unclosed_quotes(const char *line);

// Utils

char	*ft_strndup(const char *str, size_t n);

int		ft_strcmp(const char *s1, const char *s2);

void	free_split(char **arr);

char	*ft_strjoin_free(char *s1, char *s2);

char	*ft_strcpy(char *dest, const char *src);

char	*ft_strjoin_free_expand(char *s1, char *s2, int free1, int free2);

#endif