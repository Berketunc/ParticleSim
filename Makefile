# Variables
CC = g++
CFLAGS = -Wall -std=c++17
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
# Add all your .cpp files here
SRC = main.cpp Particle.cpp
# Variables
CC = g++
CFLAGS = -Wall -std=c++17
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Files - Ensure Particle.cpp is included here to fix "undefined reference" errors
SRC = main.cpp Particle.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = game

# Phony targets ensure these commands run even if a file named 'run' or 'clean' exists
.PHONY: all clean run

# Default target: builds the game and then runs it
all: $(TARGET) run

# Linking stage: combines object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compilation stage: turns each .cpp into a .o file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Execution stage: runs the binary
run:
	./$(TARGET)

# Cleanup: removes object files and the binary to allow a fresh build
clean:
	rm -f *.o $(TARGET)OBJ = $(SRC:.cpp=.o)
TARGET = game

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)
	./$(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
