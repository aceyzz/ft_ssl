# ---------------------------------------------------------------------------- #
# 								Variables									   #
# ---------------------------------------------------------------------------- #
NAME        := ft_ssl

SRCDIR      := sources
OBJDIR      := .obj
INCDIR      := includes

LIBFT_DIR   := $(INCDIR)/libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -g
INCFLAGS    := -I$(INCDIR) -I$(LIBFT_DIR)
RM          := rm -rf
MAKEFLAGS   := --no-print-directory

SRC         := $(SRCDIR)/digest.c \
				$(SRCDIR)/utils/format.c \
				$(SRCDIR)/utils/io.c \
				$(SRCDIR)/utils/log.c \
				$(SRCDIR)/main.c \
				$(SRCDIR)/algo/md5.c \
				$(SRCDIR)/parse_run/parse.c \
				$(SRCDIR)/parse_run/run.c \
				$(SRCDIR)/algo/sha256.c \
				$(SRCDIR)/algo/sha512.c \
				$(SRCDIR)/utils/tasks.c \
				$(SRCDIR)/utils/utils.c

OBJ 		:= $(OBJDIR)/digest.o \
				$(OBJDIR)/utils/format.o \
				$(OBJDIR)/utils/io.o \
				$(OBJDIR)/utils/log.o \
				$(OBJDIR)/main.o \
				$(OBJDIR)/algo/md5.o \
				$(OBJDIR)/parse_run/parse.o \
				$(OBJDIR)/parse_run/run.o \
				$(OBJDIR)/algo/sha256.o \
				$(OBJDIR)/algo/sha512.o \
				$(OBJDIR)/utils/tasks.o \
				$(OBJDIR)/utils/utils.o

CYAN		:= \033[0;36m
GREEN		:= \033[0;32m
YELLOW		:= \033[0;33m
RED			:= \033[0;31m
NC			:= \033[0m

# ---------------------------------------------------------------------------- #
# 									Regles								       #
# ---------------------------------------------------------------------------- #
all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJ) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $@
	@echo "$(NAME) créé."

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null 2>&1 || true

$(OBJDIR):
	@mkdir -p $(OBJDIR)/utils $(OBJDIR)/algo $(OBJDIR)/parse_run

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@
	@echo "Compilation de $<"

clean:
	@$(RM) $(OBJDIR) > /dev/null 2>&1 || true
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1 || true
	@echo "Objets nettoyés."

fclean: clean
	@$(RM) $(NAME) > /dev/null 2>&1 || true
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || true
	@echo "Binaire et libft.a supprimés."

re: fclean all

# test-leaks: all
# 	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) md5 -s "salut"

.PHONY: all clean fclean re
