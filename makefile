CXX = g++
FLAGS = -Wall -Werror -Wextra -std=c++20
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: all
all: main clean
	./main

main: main.o maze.o
	$(CXX) $(LFLAGS) $^ -o $@

main.o: main.cpp maze/maze.hpp
	$(CXX) $(FLAGS) -c $<

maze.o: maze/maze.cpp maze/maze.hpp
	$(CXX) $(FLAGS) -c $<

.PHONY: clean
clean:
	rm *.o