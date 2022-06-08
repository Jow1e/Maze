#pragma once

#include <vector>
#include <stack>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>


class Maze {

private:
    class cell {

    private:
        std::vector<cell*> links;

    public:
        const int rank, file;

        cell(int rank, int file): rank(rank), file(file) {}
        inline void add_link(cell* c);
        bool linked_to(const cell* c) const;
    };

    const int num_rank, num_file;
    std::vector<std::vector<cell*>> maze;
    mutable std::mt19937 mt;

    void generate();
    void draw(sf::RenderWindow &window);
    inline bool is_valid(int rank, int file) const;                 // checks if index is inside board
    inline bool is_visited(int rank, int file) const;               // checks if index was already visited during generation step
    inline bool are_linked(const cell* a, const cell* b) const;     // checks if 2 neighbour cells have no wall between
    cell* pick_free_random_neighbour(const cell* c) const;

public:
    Maze(int num_rank, int num_file);
    void play();
};
