# Compiler and flags
CC = clang
# CFLAGS = -Wall -Wextra -Werror -g3 -fno-omit-frame-pointer -fsanitize=address -fsanitize=null -fsanitize=undefined
CFLAGS = -Wall -Wextra -Werror -O3

# Source files
SRCS = main.c mlx.c

# Header files
HEADERS = raylib.h raygui.h raymath.h rlgl.h mlx.h mlx_int.h

# Output executable
NAME = game

# Libraries
LIBS = libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

# Objects
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(NAME)

# Linking
$(NAME): $(OBJS) libraylib.a
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

# Compiling
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Clean object files and executable, but keep libraylib.a
fclean: clean
	rm -f $(NAME)

# Recompile
re: fclean all

.PHONY: all clean fclean re

