NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
CPPFLAGS = -Iinclude

SOURCES = \
	src/main.c \
	src/parsing/parse_arguments.c \
	src/time/time.c \
	src/time/interruptible_sleep.c \
	src/simulation/simulation_state.c \
	src/simulation/init_simulation.c \
	src/heap/heap.c \
	src/heap/heap_compare.c \
	src/log/log.c

OBJECTS = $(SOURCES:%.c=build/%.o)
HEAP_TEST = test_heap_compare

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
	rm -f $(NAME) $(HEAP_TEST)

re: fclean all

test_heap_compare: src/heap/heap.c src/heap/heap_compare.c \
		tests/test_heap_compare.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(HEAP_TEST)
	./$(HEAP_TEST)

.PHONY: all clean fclean re test_heap_compare
