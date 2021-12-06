VER=0.5
SRC_DIR = src
OBJ_DIR = obj
BiN_DIR = bin
objects = $(OBJ_DIR)/grille.o  $(OBJ_DIR)/jeu.o $(OBJ_DIR)/io.o $(OBJ_DIR)/main.o
STATIC = static
LIB = $(STATIC)/libjeu.a
main= bin/main
src = $(SRC_DIR)/*.c  
CFLAGS = -Wall -Werror -Wextra -g -Iinclude/ 
LFLAGS = -L$(STATIC) -ljeu 
MODE=GRAPHIQUE

$(shell   mkdir -p $(OBJ_DIR) $(BiN_DIR) $(STATIC) )

ifneq "$(MODE)" "TEXTE" 
	ifneq "$(MODE)" "GRAPHIQUE"  #verification que le mode est soit GRAPHIQUE soit TEXTE
$(error Option non-acceptée utilisez MODE=GRAPHIQUE ou MODE=TEXTE)
	endif
endif

ifneq "$(MODE)" "TEXTE"   #si MODE=TEXTE pas specifié compilation avec lib cairo
CFLAGS += -I/usr/include/cairo 
LFLAGS +=  -lm -lcairo -lX11 
endif

all: $(main) 
$(main) : $(objects) $(LIB)
	gcc-9 -D$(MODE) $(OBJ_DIR)/io.o $(OBJ_DIR)/main.o -o $@ $(CFLAGS) $(LFLAGS)

doc:
	doxygen Doxyfile

$(objects): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc-9  -D$(MODE) $< -c -o $@ $(CFLAGS)

$(LIB): $(OBJ_DIR)/grille.o  $(OBJ_DIR)/jeu.o
	ar -rcs $(LIB)  $(OBJ_DIR)/grille.o  $(OBJ_DIR)/jeu.o

.PHONY : 
	clean

clean : 
	rm -rf obj bin dist $(STATIC) doc


dist: $(main)
	@mkdir -p dist
	
	tar -c --lzma -f dist/XIROUCHAKISAlexandros-GoL-$(VER).tar.xz ../jeudelavie/grilles ../jeudelavie/include ../jeudelavie/src ../jeudelavie/Makefile ../jeudelavie/Doxyfile ../jeudelavie/README.md
	
help:
	@echo "======== Help Jeu de la Vie $(VER) ========"
	@echo "./bin/main grilles/<grille> : éxécuter le programme"
	@echo "make: compiler le programme (MODE=TEXTE compile sans interface graphique sinon compile avec tous les deux options)"
	@echo "make clean: effacer les fichiers générés"
	@echo "make dist: créer une archive du projet"
	@echo "make doc: générér la documentation doxygen"
	@echo "make clean_doc: effacer la documentation"
	@echo "make help: afficher l'aide"