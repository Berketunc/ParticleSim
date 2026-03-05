# Variables
CC = g++
CFLAGS = -Wall -std=c++17
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# FILES - Make sure Solver.cpp is listed here!
SRC = main.cpp Particle.cpp collision.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = game

.PHONY: all clean run

all: $(TARGET) run

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)
