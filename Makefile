################################################################################
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#              #
#    Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr        #
#                                                                              #
################################################################################

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
	src/monitor/monitor.c \
	src/monitor/monitor_check.c \
	src/dongle/dongle_init.c \
	src/dongle/dongle_access.c \
	src/dongle/dongle_wait.c \
	src/coder/coder_init.c \
	src/coder/coder_thread.c \
	src/coder/coder_cycle.c \
	src/coder/coder_single.c \
	src/coder/coder_resources.c \
	src/heap/heap.c \
	src/heap/heap_pop.c \
	src/heap/heap_order.c \
	src/heap/heap_compare.c \
	src/log/log.c

OBJECTS = $(SOURCES:%.c=build/%.o)
HEAP_TEST = test_heap_compare

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(OBJECTS): include/codexion.h

build/src/coder/coder_thread.o build/src/coder/coder_cycle.o \
	build/src/coder/coder_single.o: include/coder_internal.h

build/src/dongle/dongle_access.o build/src/dongle/dongle_wait.o: \
	include/dongle_internal.h

build/src/monitor/monitor.o build/src/monitor/monitor_check.o: \
	include/monitor_internal.h

build/src/heap/heap.o build/src/heap/heap_pop.o \
	build/src/heap/heap_order.o: include/heap_internal.h

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build

fclean: clean
	rm -f $(NAME) $(HEAP_TEST)

re: fclean all

test_heap_compare: src/heap/heap.c src/heap/heap_pop.c \
		src/heap/heap_order.c \
		src/heap/heap_compare.c \
		tests/test_heap_compare.c tests/test_heap_remove.c \
		tests/test_heap_stress.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(HEAP_TEST)
	./$(HEAP_TEST)

.PHONY: all clean fclean re test_heap_compare
