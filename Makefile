# This tells make to always run 'run' even if a file named 'run' exists
.PHONY: all clean

all: game
	chmod +x game
	./game

game: main.cpp
	g++ main.cpp -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f game
