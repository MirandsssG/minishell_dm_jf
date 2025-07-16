CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes/ -g
AR = ar rc
NAME = minishell
SRCS = main.c \
		$(addprefix builtins/, cd_builtin.c echo_builtin.c env_builtin.c exit_builtin.c export_builtin.c pwd_builtin.c unset_builtin.c execute_builtin.c) \
		$(addprefix env/, initialize_env.c env_list_to_envp.c) \
		$(addprefix execute/, parse_and_exec.c exec_pipes.c exec_without_pipes.c free_cmds.c heredocs.c) \
		$(addprefix parsing/, tokenize.c expand_variables.c is_builtin.c parse_cmds.c has_unclosed_quotes.c) \
		$(addprefix utils/, ft_strndup.c ft_strcmp.c free_split.c ft_strjoin_free.c ft_strcpy.c ft_strjoin_free_expand.c)
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
