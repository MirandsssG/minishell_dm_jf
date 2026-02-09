CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes/ -g
AR = ar rc
NAME = minishell
SRCS = main.c \
		$(addprefix builtins/, export_builtin_helper.c cd_builtin.c echo_builtin.c env_builtin.c exit_builtin.c export_builtin.c pwd_builtin.c unset_builtin.c execute_builtin.c) \
		$(addprefix env/, initialize_env.c env_list_to_envp.c initialize_env_helpers.c) \
		$(addprefix execute/, exec_pipes_helper.c heredoc_expand.c redirects.c find_command_path.c parse_and_exec.c exec_pipes.c exec_without_pipes.c heredocs.c) \
		$(addprefix parsing/, syntax_helper.c check_syntax.c tokenize.c expand_variables.c is_builtin.c parse_cmds.c parse_cmds_helpers.c parse_cmds_helpers_2.c has_unclosed_quotes.c expand_token_helpers.c tokenize_helpers.c) \
		$(addprefix utils/, free_cmds.c signals.c check_before_execve.c ft_strndup.c ft_strcmp.c free_split.c ft_strjoin_free.c ft_strcpy.c ft_strjoin_free_expand.c)
OBJ_DIR = objects
OBJ = $(SRCS:.c=.o)
OBJ := $(addprefix $(OBJ_DIR)/, $(OBJ))
LIBFT_DIR = includes/libft
LIBFT = includes/libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT) -lreadline

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@make clean -C ./$(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)
	@make fclean -C ./$(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re comp
