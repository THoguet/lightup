CC=gcc                      # compilateur
CFLAGS=-Wall -std=c99    # options de compilation
LDFLAGS=-L.                    # options de link
LDLIBS=-lgame                 # bibliothèques
CPPFLAGS=                   # options de preprocessing
AR= ar

all: game_text

libgame.a: game_aux.o game.o
	$(AR) rcs $@ $^

game_text: game_text.o libgame.a
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: clean dep

include dep

# génération des dépendances
dep:
	$(CC) -MM *.c > dep
	
clean:
	rm -f dep game_aux.o *~  game_text game_text.o libgame.a