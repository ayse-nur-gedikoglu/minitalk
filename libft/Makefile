# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agedikog <agedikog@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/09 13:29:40 by agedikog          #+#    #+#              #
#    Updated: 2025/02/09 13:29:40 by agedikog         ###   ########.fr        #
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

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $@ $^
	@echo "Libft created successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

clean:
	@rm -rf $(OBJDIR)
	@echo "Cleared libft temporary files."

fclean: clean
	@rm -f $(NAME)
	@echo "Libft completely cleaned."

re: fclean all

.PHONY: all clean fclean re
