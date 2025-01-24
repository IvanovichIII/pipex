# *************************************************************************** #
#                                                                             #
#                             🎸 PIPEX MAKEFILE 🎸                           #
#                                                                             #
# *************************************************************************** #

# Nombre del ejecutable
NAME = pipex

# Compilador y banderas
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

# Archivos fuente y objeto
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Librerías
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
GNL_DIR = ./gnl
GNL = $(GNL_DIR)/libftgnl.a
PRINTF_DIR = ./printf
PRINTF = $(PRINTF_DIR)/libftprintf.a

# Colores para los mensajes
GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

# Emojis (¡porque sí!)
CHECK = ✅
CROSS = ❌
BUILD = 🚀
CLEAN = 🧹
COMPILE = 🔧
DONE = 🎉

# Reglas principales
all: banner $(NAME)

banner:
	@echo "$(CYAN)🎸 Let's rock! Compiling pipex... $(RESET)"

$(NAME): $(OBJS) $(LIBFT) $(GNL) $(PRINTF)
	@echo "$(COMPILE) $(YELLOW)Linking everything together...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(GNL) $(PRINTF)
	@echo "$(CHECK) $(GREEN)Compilation successful!$(RESET)"

%.o: %.c
	@echo "$(COMPILE) $(CYAN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "$(BUILD) $(YELLOW)Building Libft...$(RESET)"
	@make -C $(LIBFT_DIR)

$(GNL):
	@echo "$(BUILD) $(YELLOW)Building Gnl...$(RESET)"
	@make -C $(GNL_DIR)

$(PRINTF):
	@echo "$(BUILD) $(YELLOW)Building Printf...$(RESET)"
	@make -C $(PRINTF_DIR)

# Limpiar archivos intermedios
clean:
	@echo "$(CLEAN) $(RED)Removing object files...$(RESET)"
	@$(RM) $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(GNL_DIR)
	@make clean -C $(PRINTF_DIR)
	@echo "$(CROSS) $(RED)Cleaned!$(RESET)"

# Limpieza total
fclean: clean
	@echo "$(CLEAN) $(RED)Removing executable...$(RESET)"
	@$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(GNL_DIR)
	@make fclean -C $(PRINTF_DIR)
	@echo "$(CROSS) $(RED)Full clean done!$(RESET)"

# Recompilar desde cero
re: fclean all

# Mensajes adicionales
.PHONY: all clean fclean re banner

bonus:
	@echo "$(YELLOW)No hay bonus... ¡pero podrías añadirlos tú! 😉$(RESET)"

# 🎉 Al final...
end:
	@echo "$(DONE) $(GREEN)Pipex listo para el rock and roll. ¡Ánimo!$(RESET)"
