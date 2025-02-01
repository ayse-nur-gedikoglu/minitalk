NAME = minitalk
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CLIENT = client
SERVER = server

SRC_CLIENT = $(SRCDIR)/client.c
SRC_SERVER = $(SRCDIR)/server.c

OBJ_CLIENT = $(OBJDIR)/client.o
OBJ_SERVER = $(OBJDIR)/server.o

# Renkli mesajlar
GREEN = \033[0;32m
RESET = \033[0m

all: libft $(CLIENT) $(SERVER)

$(CLIENT): $(OBJ_CLIENT)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(CLIENT) $(OBJ_CLIENT) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Client compiled successfully!$(RESET)"

$(SERVER): $(OBJ_SERVER)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(SERVER) $(OBJ_SERVER) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Server compiled successfully!$(RESET)"

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
	@rm -f $(CLIENT) $(SERVER)
	@make -C $(LIBFT_DIR) fclean
	@echo "All files cleared."

re: fclean all

.PHONY: all clean fclean re libft