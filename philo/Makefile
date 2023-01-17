# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: sbos <sbos@student.codam.nl>                 +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/31 17:51:35 by sbos          #+#    #+#                  #
#    Updated: 2022/11/03 15:00:49 by sbos          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

################################################################################

NAME := philo

CC := cc

OBJ_DIR := obj

# TODO: Remove extra flags
CFLAGS := -Wall -Wextra -Werror -Wconversion -Wpedantic -Wfatal-errors -pthread

################################################################################

LIBS :=

################################################################################

SOURCES :=\
src/init/atoi_safe/atoi_range.c\
src/init/atoi_safe/utils.c\
src/init/atoi_safe.c\
src/init/init.c\
src/run/run_philosopher.c\
src/run/main_loop.c\
src/utils/destroy.c\
src/utils/mutex.c\
src/utils/print_error.c\
src/utils/print_event.c\
src/utils/join_philosophers.c\
src/utils/time.c\
src/main.c

################################################################################

HEADERS +=\
src/init/atoi_safe/ph_private_atoi_safe.h\
src/init/ph_init.h\
src/run/ph_run.h\
src/utils/ph_utils.h\
src/ph_typedefs.h\
src/philo.h

################################################################################

ifdef O3
CFLAGS += -Ofast
endif

ifdef DEBUG
CFLAGS += -g3
endif

ifdef SAN
CFLAGS += -fsanitize=address
endif

################################################################################

FCLEANED_FILES := $(NAME)

OBJECT_PATHS := $(addprefix $(OBJ_DIR)/,$(SOURCES:.c=.o))

################################################################################

# Only cleans when MAKE_DATA changes.
MAKE_DATA := $(shell uname) $(CFLAGS) $(LIBS) $(SOURCES)
DATA_FILE := .make_data

ifneq ($(shell echo "$(MAKE_DATA)"), $(shell cat "$(DATA_FILE)" 2> /dev/null))
PRE_RULES := clean
endif

################################################################################

INCLUDES := $(sort $(addprefix -I, src))

################################################################################

.PHONY: all
all: $(PRE_RULES) $(NAME)

################################################################################

$(NAME): $(OBJECT_PATHS)
	$(CC) $(CFLAGS) $(OBJECT_PATHS) $(LIBS) -o $(NAME)
	@echo "$(MAKE_DATA)" > $(DATA_FILE)

################################################################################

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

################################################################################

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/

.PHONY: fclean
fclean: clean
	rm -f $(FCLEANED_FILES)

.PHONY: re
re: fclean all

################################################################################
