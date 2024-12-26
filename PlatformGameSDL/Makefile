# Variabler för kompilatorn och flaggor
CC=gcc-14
CFLAGS=-c -g -I/opt/homebrew/include/SDL2 -I/opt/homebrew/include/SDL2_image -I/opt/homebrew/include/SDL2_ttf -I/opt/homebrew/include/SDL2_mixer
LDFLAGS=-I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Filnamn
OBJ=main.o map.o player.o enemy.o
EXEC=PlatformGameSDL

# Länkning
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Kompilera källkod
main.o: source/main.c
	$(CC) $(CFLAGS) source/main.c -o main.o

map.o: source/map.c
	$(CC) $(CFLAGS) source/map.c -o map.o

player.o: source/player.c
	$(CC) $(CFLAGS) source/player.c -o player.o

enemy.o: source/enemy.c
	$(CC) $(CFLAGS) source/enemy.c -o enemy.o
# Rensa binärer
clean:
	rm -f *.o $(EXEC)

# Kör programmet
run: $(EXEC)
	./$(EXEC)