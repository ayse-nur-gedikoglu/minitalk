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

NAME = libftprintf.a

SRCS= ft_printf.c ft_printf_hex.c ft_printf_alnum.c

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
AR = ar -rc

OBJS = $(SRCS:.c=.o)

all: ${NAME}

${NAME}: ${OBJS}
	${AR} ${NAME} ${OBJS}

run: ${NAME} $(OBJS)
	${CC} ${CFLAGS} ${NAME} $(OBJS)

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: fclean clean all run
