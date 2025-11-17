# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: CarlosJX7                                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/13 19:12:35 by CarlosJX7         #+#    #+#              #
#    Updated: 2025/11/13 19:12:35 by CarlosJX7        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -pthread
RM			= rm -f

# Directories
SRC_DIR		= src
HEADER_DIR	= header
OBJ_DIR		= obj

# Source files
SRC_FILES	= main.c \
			  aux.c \
			  initialization.c \
			  philo_simulation.c \
			  utils.c \
			  simulation_aux.c

# Object files
SRCS		= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Header files
HEADERS		= $(HEADER_DIR)/philo.h \
			  $(HEADER_DIR)/aux.h \
			  $(HEADER_DIR)/initialization.h \
			  $(HEADER_DIR)/philo_simulation.h \
			  $(HEADER_DIR)/utils.h \
			  $(HEADER_DIR)/simulation_aux.h

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@
	@echo "$(GREEN)Compiled: $<$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "$(RED)✓ Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)✓ $(NAME) removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re