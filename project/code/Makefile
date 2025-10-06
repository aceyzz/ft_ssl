# ---------------------------------------------------------------------------- #
# 								Variables									   #
# ---------------------------------------------------------------------------- #
NAME        := ft_ssl

SRCDIR      := sources
OBJDIR      := .obj
INCDIR      := includes

CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -g
INCFLAGS    := -I$(INCDIR) -I$(LIBFT_DIR)
RM          := rm -rf
MAKEFLAGS   := --no-print-directory
SRC         := $(shell find $(SRCDIR) -name '*.c')
OBJ         := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

CYAN		:= \033[0;36m
GREEN		:= \033[0;32m
YELLOW		:= \033[0;33m
RED			:= \033[0;31m
NC			:= \033[0m

# ---------------------------------------------------------------------------- #
# 									Regles								       #
# ---------------------------------------------------------------------------- #
all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "$(NAME) créé."

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@
	@echo "Compilation de $<"

clean:
	@$(RM) $(OBJDIR) > /dev/null 2>&1 || true
	@echo "Objets nettoyés."

fclean: clean
	@$(RM) $(NAME) > /dev/null 2>&1 || true
	@echo "Binaire supprimé."

re: fclean all

# leaks: all
# 	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re
