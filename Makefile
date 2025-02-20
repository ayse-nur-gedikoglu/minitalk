# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agedikog <agedikog@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/10 15:56:21 by agedikog          #+#    #+#              #
#    Updated: 2025/02/10 15:56:21 by agedikog         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minitalk
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

SRCDIR = mandotory
SRCBDIR = bonus
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CLIENT = client
SERVER = server
CLIENT_BONUS = client_bonus
SERVER_BONUS = server_bonus

SRC_CLIENT = $(SRCDIR)/client.c
SRC_SERVER = $(SRCDIR)/server.c
SRC_CLIENT_BONUS = $(SRCBDIR)/client_bonus.c
SRC_SERVER_BONUS = $(SRCBDIR)/server_bonus.c

OBJ_CLIENT = $(OBJDIR)/client.o
OBJ_SERVER = $(OBJDIR)/server.o
OBJ_CLIENT_BONUS = $(OBJDIR)/client_bonus.o
OBJ_SERVER_BONUS = $(OBJDIR)/server_bonus.o

OBJS = $(OBJ_CLIENT) $(OBJ_SERVER)
OBJS_BONUS = $(OBJ_CLIENT_BONUS) $(OBJ_SERVER_BONUS)

all: libft $(CLIENT) $(SERVER)
	@echo "All files are up to date."

bonus: libft | $(OBJDIR)
	@$(MAKE) $(CLIENT_BONUS)
	@$(MAKE) $(SERVER_BONUS)
	@echo "Bonus files are up to date."

$(CLIENT): $(OBJ_CLIENT) $(LIBFT) | $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(CLIENT) $(OBJ_CLIENT) -L$(LIBFT_DIR) -lft
	@echo "Client compiled successfully!"

$(SERVER): $(OBJ_SERVER) $(LIBFT) | $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(SERVER) $(OBJ_SERVER) -L$(LIBFT_DIR) -lft
	@echo "Server compiled successfully!"

$(CLIENT_BONUS): $(OBJ_CLIENT_BONUS) $(LIBFT) | $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(CLIENT_BONUS) $(OBJ_CLIENT_BONUS) -L$(LIBFT_DIR) -lft
	@echo "Client Bonus compiled successfully!"

$(SERVER_BONUS): $(OBJ_SERVER_BONUS) $(LIBFT) | $(OBJDIR)
	@$(CC) $(CFLAGS) -o $(SERVER_BONUS) $(OBJ_SERVER_BONUS) -L$(LIBFT_DIR) -lft
	@echo "Server Bonus compiled successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCBDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

libft: $(LIBFT)
	@make -C $(LIBFT_DIR) --silent

$(LIBFT):
	@make -C $(LIBFT_DIR) --silent

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFT_DIR) clean --silent
	@echo "Temporary files cleared."

fclean: clean
	@rm -f $(CLIENT) $(SERVER) $(CLIENT_BONUS) $(SERVER_BONUS)
	@make -C $(LIBFT_DIR) fclean --silent
	@echo "All files cleared."

re: fclean all

.PHONY: all clean fclean re libft bonus
	