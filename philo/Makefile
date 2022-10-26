# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: sbos <sbos@student.codam.nl>                 +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/31 17:51:35 by sbos          #+#    #+#                  #
#    Updated: 2022/09/01 16:11:12 by sbos          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

################################################################################

NAME := philo

CC := cc

OBJ_DIR := obj

CFLAGS := -Wall -Wextra -Werror -Wconversion -Wpedantic -Wfatal-errors -pthread

################################################################################

LIBS :=

################################################################################

SOURCES :=\
src/atoi_safe/private/atoi_range.c\
src/atoi_safe/private/utils.c\
src/atoi_safe/atoi_safe.c\
src/main.c

################################################################################

HEADERS +=\
src/atoi_safe/private/ph_private_atoi_safe.h\
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
