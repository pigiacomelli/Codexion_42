NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
CPPFLAGS = -Iinclude

SOURCES = \
	src/main.c \
	src/parsing/parse_arguments.c \
	src/time/time.c \
	src/simulation/simulation_state.c \
	src/simulation/init_simulation.c \
	src/log/log.c

OBJECTS = $(SOURCES:%.c=build/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(OBJECTS): include/codexion.h

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
