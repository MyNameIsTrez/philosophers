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
src/init/parse_size/atoi_range.c\
src/init/parse_size/utils.c\
src/init/init.c\
src/init/parse_size.c\
src/init/philosophers.c\
src/run/run_philosopher.c\
src/run/run.c\
src/utils/destroy.c\
src/utils/join_philosophers.c\
src/utils/mutex.c\
src/utils/print_error.c\
src/utils/print_event.c\
src/utils/should_stop.c\
src/utils/strlen.c\
src/utils/time.c\
src/main.c

################################################################################

HEADERS +=\
include/ph_declarations.h\
include/ph_enums.h\
include/ph_parse_size.h\
include/ph_structs.h\
include/philo.h

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

INCLUDES := $(sort $(addprefix -I, include))

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
