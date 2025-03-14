# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 11:10:11 by adpachec          #+#    #+#              #
#    Updated: 2025/03/13 19:59:09 by jutrera-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME		=	rt

# Paths
O_DIR		=	./obj
I_DIR		=	./include
S_DIR		=	./srcs

# Source files
SRCS		=	./srcs/color.c \
				./srcs/cone.c \
				./srcs/cone2.c \
				./srcs/cylinder.c \
				./srcs/cylinder2.c \
				./srcs/ft_atoi.c \
				./srcs/ft_atol.c \
				./srcs/ft_gnl.c \
				./srcs/ft_split.c  \
				./srcs/ft_strlen.c \
				./srcs/ft_strtrim.c \
				./srcs/light_shadow.c \
				./srcs/main.c \
				./srcs/my_hooks.c \
				./srcs/parser_gets.c \
				./srcs/parser_lists.c \
				./srcs/parser_loads.c \
				./srcs/parser_utils.c \
				./srcs/parser.c \
				./srcs/plane.c \
				./srcs/print_errors.c \
				./srcs/process_file.c \
				./srcs/process_img.c \
				./srcs/ray.c \
				./srcs/run_rt.c \
				./srcs/sphere.c \
				./srcs/triangle.c \
				./srcs/vectors1.c \
				./srcs/vectors2.c \
				./srcs/vectors3.c
			
# Compiler and flags
CC			=	gcc
RM 			=	rm -rf
CFLAGS		=	-Wall -Wextra -Werror -Wpedantic -g3 -Wshadow
LEAKS		=	-fsanitize=address -g
MLXFLAG		= 	-Lusr/lib -Lmlx -lmlx -lXext -lX11 -lm -lbsd -Imlx
CAIRO		=	$(shell pkg-config --cflags --libs gtk+-3.0)

#  Colours
RED 		= 	\033[0;31m
GREEN	 	= 	\033[0;32m
YELLOW 		= 	\033[0;33m
RESET 		= 	\033[0m

# Delete files
RM 			= 	rm -rf

# Object files
OBJS 		= 	$(patsubst $(S_DIR)/%.c, $(O_DIR)/%.o, $(SRCS))

# Default target
all			:	$(NAME)

# Compile object files
$(O_DIR)/%.o: 	$(S_DIR)/%.c
				@mkdir -p $(dir $@)
				@printf "Compiling $(YELLOW)$<$(RESET)\r"
				@$(CC) $(CFLAGS) $(LEAKS) -I$(I_DIR) -c $< -o $@ $(CAIRO)
				@printf "                                                        \r"

# Link program
$(NAME)		: 	$(OBJS)
				@echo "Compiling... $(GREEN)OK!$(RESET)"
				@echo "Linking object files... \c"
				@$(CC) $(OBJS) $(CFLAGS) $(LEAKS) $(MLXFLAG) -o $(NAME) $(CAIRO)
				@echo "$(GREEN)OK!$(RESET)"
				@echo "Program $(YELLOW)rt$(GREEN) created successfully !\n$(RESET)"

# Clean object files
clean		:
				@echo "$(RED)Deleting object files... $(RESET)\c"
				@$(RM) $(O_DIR)
				@echo "$(GREEN)OK!$(RESET)"

# Clean everything and recompile
fclean		: 	clean
				@echo "$(RED)Deleting object and binary files... $(RESET)\c"
				@$(RM) $(NAME)
				@echo "$(GREEN)OK!$(RESET)"

# Recompile everything
re			: 	fclean all

# Phony targets
.PHONY		:	all clean fclean re