# Variables
NAME = ft_ping
CC = gcc -g -O0
CFLAGS = -Wall -Wextra -Werror
SRC_DIR = srcs
SRCS = $(SRC_DIR)/ft_ping.c \
	   $(SRC_DIR)/errors.c \
	   $(SRC_DIR)/utils.c \
	   $(SRC_DIR)/parsing.c \
	   $(SRC_DIR)/treatment.c \
	   $(SRC_DIR)/signal.c
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INCLUDES = -Iheaders

# Règles
all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d $(OBJ_DIR) ]; then rmdir --ignore-fail-on-non-empty $(OBJ_DIR); fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
