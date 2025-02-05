NAME = minitalk
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CLIENT = client
SERVER = server
CLIENT_BONUS = client_bonus
SERVER_BONUS = server_bonus

SRC_CLIENT = $(SRCDIR)/client.c
SRC_SERVER = $(SRCDIR)/server.c
SRC_CLIENT_BONUS = $(SRCDIR)/client_bonus.c
SRC_SERVER_BONUS = $(SRCDIR)/server_bonus.c

OBJ_CLIENT = $(OBJDIR)/client.o
OBJ_SERVER = $(OBJDIR)/server.o
OBJ_CLIENT_BONUS = $(OBJDIR)/client_bonus.o
OBJ_SERVER_BONUS = $(OBJDIR)/server_bonus.o

# Renkler
GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

all: libft $(CLIENT) $(SERVER)

bonus: libft $(CLIENT_BONUS) $(SERVER_BONUS)

$(CLIENT): $(OBJ_CLIENT)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(CLIENT) $(OBJ_CLIENT) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Client compiled successfully!$(RESET)"

$(SERVER): $(OBJ_SERVER)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(SERVER) $(OBJ_SERVER) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Server compiled successfully!$(RESET)"

$(CLIENT_BONUS): $(OBJ_CLIENT_BONUS)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(CLIENT_BONUS) $(OBJ_CLIENT_BONUS) -L$(LIBFT_DIR) -lft
	@echo "$(CYAN)Client Bonus compiled successfully!$(RESET)"

$(SERVER_BONUS): $(OBJ_SERVER_BONUS)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(SERVER_BONUS) $(OBJ_SERVER_BONUS) -L$(LIBFT_DIR) -lft
	@echo "$(CYAN)Server Bonus compiled successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiled $< successfully!$(RESET)"

libft:
	@make -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(RED)Temporary files cleared.$(RESET)"

fclean: clean
	@rm -f $(CLIENT) $(SERVER) $(CLIENT_BONUS) $(SERVER_BONUS)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(RED)All files cleared.$(RESET)"

re: fclean all

.PHONY: all clean fclean re libft bonus