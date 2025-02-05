# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agedikog <gedikoglu_27@icloud.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/07 11:57:12 by agedikog          #+#    #+#              #
#    Updated: 2024/11/11 14:13:56 by agedikog         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

SRCDIR = src
OBJDIR = obj
HEADER_DIR = inc

SRC = $(SRCDIR)/ft_atoi.c \
      $(SRCDIR)/ft_printf_alnum.c \
      $(SRCDIR)/ft_printf.c

OBJ = $(OBJDIR)/ft_atoi.o \
      $(OBJDIR)/ft_printf_alnum.o \
      $(OBJDIR)/ft_printf.o

GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $@ $^
	@echo "$(GREEN)Libft created successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CYAN)Compiled $< successfully!$(RESET)"

clean:
	@rm -rf $(OBJDIR)
	@echo "$(YELLOW)Cleared libft temporary files.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Libft completely cleaned.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
