NAME = minitalk
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Renkli mesajlar
GREEN = \033[0;32m
RESET = \033[0m

all: libft $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Minitalk compiled successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

libft:
	@make -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFT_DIR) clean
	@echo "Temporary files cleared."

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "All files cleared."

re: fclean all

.PHONY: all clean fclean re libft