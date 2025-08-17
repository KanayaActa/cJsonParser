NAME = json_parser

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iincs

# ソースファイル
SRCS_DIR = srcs
UTILS_DIR = srcs/utils
HANDLE_STATE_DIR = srcs/handle_state

# メインのソースファイル
SRCS = \
	main.c \
	$(SRCS_DIR)/json_lexer.c \
	$(SRCS_DIR)/json_lexer_utils.c \
	$(SRCS_DIR)/json_lexer_utils2.c \
	$(SRCS_DIR)/json_parser_stack.c \
	$(SRCS_DIR)/json_parser_utils.c \
	$(SRCS_DIR)/json_value.c \
	$(SRCS_DIR)/json_free.c \
	$(SRCS_DIR)/json_insert.c \
	$(SRCS_DIR)/operate_stack.c \
	$(SRCS_DIR)/init.c

# ユーティリティ関数
UTILS_SRCS = \
	$(UTILS_DIR)/ft_strlen.c \
	$(UTILS_DIR)/ft_memcpy.c \
	$(UTILS_DIR)/ft_memset.c \
	$(UTILS_DIR)/ft_bzero.c \
	$(UTILS_DIR)/ft_strdup.c \
	$(UTILS_DIR)/ft_strncmp.c \
	$(UTILS_DIR)/ft_isspace.c \
	$(UTILS_DIR)/ft_isalpha.c \
	$(UTILS_DIR)/ft_isdigit.c \
	$(UTILS_DIR)/ft_isupper.c \
	$(UTILS_DIR)/ft_islower.c \
	$(UTILS_DIR)/ft_tolower.c \
	$(UTILS_DIR)/ft_calloc.c \
	$(UTILS_DIR)/ft_strncasecmp.c \
	$(UTILS_DIR)/ft_putstr_fd.c \
	$(UTILS_DIR)/ft_strtod.c \
	$(UTILS_DIR)/ft_strtod_utils.c \
	$(UTILS_DIR)/xrealloc.c \
	$(UTILS_DIR)/print_error.c

# 状態処理関数
HANDLE_STATE_SRCS = \
	$(HANDLE_STATE_DIR)/handle_state_value.c \
	$(HANDLE_STATE_DIR)/handle_state_value2.c \
	$(HANDLE_STATE_DIR)/handle_state_end.c \
	$(HANDLE_STATE_DIR)/handle_state_eof.c \
	$(HANDLE_STATE_DIR)/handle_state_invalid.c \
	$(HANDLE_STATE_DIR)/handle_state_object_colon.c \
	$(HANDLE_STATE_DIR)/handle_state_object_key_or_end.c \
	$(HANDLE_STATE_DIR)/handle_state_object_next_or_end.c \
	$(HANDLE_STATE_DIR)/handle_state_object_value.c \
	$(HANDLE_STATE_DIR)/handle_state_object_value2.c \
	$(HANDLE_STATE_DIR)/handle_state_array_next_or_end.c \
	$(HANDLE_STATE_DIR)/handle_state_array_value.c \
	$(HANDLE_STATE_DIR)/handle_state_array_value2.c \
	$(HANDLE_STATE_DIR)/handle_state_array_value_or_end.c

# すべてのソースファイル
ALL_SRCS = $(SRCS) $(UTILS_SRCS) $(HANDLE_STATE_SRCS)

# オブジェクトファイル
OBJS = $(ALL_SRCS:.c=.o)

# デフォルトターゲット
all: $(NAME)

# メインターゲット
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS)

# オブジェクトファイルの作成
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# クリーンアップ
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

# 再ビルド
re: fclean all

# ヘルプ
help:
	@echo "Available targets:"
	@echo "  all     - Build json_parser"
	@echo "  clean   - Remove object files"
	@echo "  fclean  - Remove object files and executable"
	@echo "  re      - Rebuild everything"
	@echo "  help    - Show this help"

.PHONY: all clean fclean re help
