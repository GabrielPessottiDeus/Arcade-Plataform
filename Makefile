##
## EPITECH PROJECT, 2025
## arcade
## File description:
## Makefile
##

# ===== CONFIGURATION =====
CC       = g++
CFLAGS   = -W -Wall -Wextra -fPIC -std=c++17
LDFLAGS  = -ldl -lncursesw
RM       = rm -f
MKDIR    = mkdir -p
NAME     = arcade
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SDL_LIBS = -lSDL2 -lSDL2_ttf

# ===== PATHS =====
SRC_DIR   = core
CORE_DIR  = core
GAMES_DIR = games
GRAPH_DIR = graphical
INC_DIR   = include
LIB_DIR   = lib
GAMESSO_DIR = gamesSo

# ===== SOURCES =====
SRC_MAIN  = test_main.cpp
SRC_CORE  = $(CORE_DIR)/Core.cpp $(CORE_DIR)/DLLoader.cpp
SRC_NCURSE= $(GRAPH_DIR)/ncurse/NCurses.cpp
SRC_SNAKE = $(GAMES_DIR)/Snake.cpp
SRC_MINESWEEPER = $(GAMES_DIR)/Minesweeper.cpp
SRC_SFML = $(GRAPH_DIR)/SFML/SFML.cpp
SRC_SDL = $(GRAPH_DIR)/sdl/SDL.cpp

# ===== TARGETS =====
all: $(NAME) $(LIB_DIR)/libncurse.so $(GAMESSO_DIR)/libsnake.so $(GAMESSO_DIR)/libminesweeper.so $(LIB_DIR)/libsfml.so $(LIB_DIR)/libsdl.so

$(NAME): $(SRC_MAIN) $(SRC_CORE)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(SFML_LIBS) $(SDL_LIBS)

# Ajouter la règle pour SFML
$(LIB_DIR)/libsfml.so: $(SRC_SFML)
	@$(MKDIR) $(LIB_DIR)
	$(CC) $(CFLAGS) -shared $< -o $@ $(SFML_LIBS)

$(LIB_DIR)/libsdl.so: $(GRAPH_DIR)/sdl/SDL.cpp
	@$(MKDIR) $(LIB_DIR)
	$(CC) $(CFLAGS) -shared $< -o $@ $(SDL_LIBS)

$(LIB_DIR)/libncurse.so: $(SRC_NCURSE)
	@$(MKDIR) $(LIB_DIR)
	$(CC) $(CFLAGS) -shared $< -o $@ $(LDFLAGS)

$(GAMESSO_DIR)/libsnake.so: $(SRC_SNAKE)
	@$(MKDIR) $(GAMESSO_DIR)
	$(CC) $(CFLAGS) -shared $< -o $@

$(GAMESSO_DIR)/libminesweeper.so: $(SRC_MINESWEEPER)
	@$(MKDIR) $(GAMESSO_DIR)
	$(CC) $(CFLAGS) -shared $< -o $@

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME) $(LIB_DIR)/*.so
	@rmdir $(LIB_DIR) 2>/dev/null || true

	$(RM) $(NAME) $(GAMESSO_DIR)/*.so
	@rmdir $(GAMESSO_DIR) 2>/dev/null || true

re: fclean all

.PHONY: all clean fclean re
