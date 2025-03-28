OBJ = build/main.o build/button.o build/panel.o build/app.o

# Detect OS
OS := $(shell uname -s 2>/dev/null)
ifeq ($(OS),)
  OS := Windows_NT
endif

ifeq ($(OS), Darwin)
    # macOS Settings
    CC = clang
    CFLAGS = -fsanitize=address -fsanitize=undefined -g -c -Wall -Wextra \
             -I/opt/homebrew/include/SDL2 \
             -I/opt/homebrew/include/SDL2_image \
             -I/opt/homebrew/include/SDL2_ttf \
             -I/opt/homebrew/include/SDL2_mixer \
             -I/opt/homebrew/include/SDL2_net
    LDFLAGS = -fsanitize=address \
              -L/opt/homebrew/lib \
              -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf \
              -lSDL2_mixer -lSDL2_net
    EXEC = build/main
    SRCDIR = source
    REMOV = rm -f build/*.o $(EXEC)
    PREFORM = MallocNanoZone=0 
else ifeq ($(OS), Windows_NT)
    # Windows Settings
    CC = gcc
    INCLUDE = C:/msys64/mingw64/include/SDL2
    CFLAGS = -g -c -I$(INCLUDE)
    LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net
    EXEC = build/main.exe
    SRCDIR = source
    REMOV = del /f build\*.o & del /f build\main.exe
    PREFORM = 
endif

# Linking
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Rules for object files
build/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $< -o $@

build/app.o: $(SRCDIR)/app.c
	$(CC) $(CFLAGS) $< -o $@

build/button.o: $(SRCDIR)/lib/button.c
	$(CC) $(CFLAGS) $< -o $@

build/panel.o: $(SRCDIR)/lib/panel.c
	$(CC) $(CFLAGS) $< -o $@

# Clean
clean:
	$(REMOV)

# Run
run: $(EXEC)
	$(PREFORM) ./$(EXEC)
