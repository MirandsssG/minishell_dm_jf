/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafonso <tafonso@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:56:59 by mirandsssg        #+#    #+#             */
/*   Updated: 2026/01/17 17:44:21 by tafonso          ###   ########.fr       */
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
# include <sys/stat.h>

# include "includes/libft/libft.h"

extern volatile sig_atomic_t	g_exit_signal;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	int				infile_fd;
	char			*outfile;
	int				append;
	int				heredoc;
	char			**heredoc_delim;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	char		*input;
	char		**tokens;
	size_t		k;
	size_t		j;
	int			var_start;
	char		*var_name;
	size_t		var_i;
	t_env		*env_list;
	t_cmd		*cmd;
	int			should_exit;
	int			last_exit_status;
	int			i;
	int			in_single;
	int			in_double;
}	t_data;

// Builtins

int		execute_builtin(t_data *data, t_cmd *cmd);

int		execute_builtin_with_redirections(t_data *data, t_cmd *cmd);

int		cd_builtin(t_data *data, t_cmd *cmd);

int		echo_builtin(t_cmd *cmd);

int		env_builtin(t_data *data);

int		exit_builtin(t_data *data, t_cmd *cmd);

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

char	*get_env_value(const char *key, t_env *env);

// Execute

char	*find_command_path(char *cmd, char **envp);

char	*search_path(char **paths, char *cmd);

char	*get_path_from_env(char **envp);

char	*ft_strjoin3(char *str1, char *separator, char *str2);

void	parse_and_exec(t_data *data);

void	execute(t_data *data, t_cmd *cmds);

void	exec_pipes(t_data *data, t_cmd *cmds);

void	exec_without_pipes(t_data *data, t_cmd *cmds);

void	free_cmds(t_cmd *cmd);

int		process_heredocs(t_cmd *cmds, t_data *data);

int		heredoc_infile(t_cmd *cmd);

int		redirection_infile(t_cmd *cmd);

int		redirection_outfile(t_cmd *cmd);

char	*expand_heredoc_line(const char *line, t_data *data);

char	*get_heredoc_delim(const char *delim, int *expand);

void	write_heredoc(char *line, int fd, int expand, t_data *data);

// Parsing

void	tokenize_inputs(t_data *data);

void	expand_variables(t_data *data);

int		is_builtin(const char *cmd);

t_cmd	*parse_cmds(char **tokens);

int		has_unclosed_quotes(const char *line);

int		handle_quotes(char c, t_data *data);

int		handle_special_dollar(const char *token, t_data *data,
			char **result, int *i);

int		handle_env_var(const char *token, t_data *data,
			char **result, int *i);

void	process_char(const char *token, t_data *data,
			char **result, int *i);

void	init_tokens(t_data *data);

int		handle_double_operator(t_data *data);

int		handle_single_operator(t_data *data);

char	*read_part(t_data *data);

void	handle_word(t_data *data);

int		is_special(char c);

int		is_space(char c);

// Syntax

int		check_syntax(char **tokens, t_data *data);

int		is_pipe(const char *t);

int		is_redirection(const char *t);

int		is_operator(const char *t);

int		is_unsupported(const char *t);

// Utils

char	*ft_strndup(const char *str, size_t n);

int		ft_strcmp(const char *s1, const char *s2);

void	free_split(char **arr);

char	*ft_strjoin_free(char *s1, char *s2);

char	*ft_strcpy(char *dest, const char *src);

char	*ft_strjoin_free_expand(char *s1, char *s2, int free1, int free2);

void	ctrlc_handler(int sig);

int		is_directory(char *path);

void	sig_heredoc(int sig);

int		sig_ctrl(t_data *data);

#endif