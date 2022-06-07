#include "maze.hpp"


using std::vector;


// class `cell`
inline void Maze::cell::add_link(cell* c) {
    links.push_back(c);
}

bool Maze::cell::linked_to(const cell* c) const {
    for (cell* l: links)
        if (l == c)
            return true;

    return false;
}


// class `Maze`
inline bool Maze::is_valid(int rank, int file) const {
    return 0 <= rank && rank < num_rank && 0 <= file && file < num_file;
}

inline bool Maze::is_visited(int rank, int file) const {
    return maze[rank][file] != nullptr;
}

inline bool Maze::are_linked(const cell* a, const cell* b) const {
    return a->linked_to(b) || b->linked_to(a);
}

Maze::cell* Maze::pick_free_random_neighbour(const cell* c) const {
    vector<cell*> choices;

    for (int irank = -1; irank <= 1; ++irank) {
        for (int ifile = -1; ifile <= 1; ++ifile) {
            if (irank * irank + ifile * ifile == 2)     // math trick to use only left, right, up and down neighbour cells
                continue;
            
            int rank = c->rank + irank;
            int file = c->file + ifile;

            if (is_valid(rank, file) && !is_visited(rank, file))
                choices.push_back(new cell(rank, file));
        }
    }

    if (!choices.empty()) {
        int index = mt() % choices.size();
        return choices[index];
    }

    return nullptr;
}

Maze::Maze(int num_rank, int num_file): num_rank(num_rank), num_file(num_file) {
    mt.seed(time(nullptr));
    maze.resize(num_rank, vector<cell*>(num_file, nullptr));
}

void Maze::generate() {
    std::stack<cell*> path;
    maze[0][0] = new cell(0, 0);
    path.push(maze[0][0]);

    while (!path.empty()) {
        cell* last = path.top();
        cell* new_cell = pick_free_random_neighbour(last);

        if (new_cell == nullptr) {
            path.pop();
            continue;
        }

        last->add_link(new_cell);
        maze[new_cell->rank][new_cell->file] = new_cell;
        path.push(new_cell);
    }
}

void Maze::draw(sf::RenderWindow &window) {
    int tile_size = 40;
    int gap_size = 2;

    for (int x = 0; x < num_file; ++x) {
        for (int y = 0; y < num_rank; ++y) {
            int rect_width = tile_size;
            int rect_height = tile_size;

            if (y + 1 != num_rank && are_linked(maze[y][x], maze[y + 1][x]))
                rect_height += gap_size;

            if (x + 1 != num_file && are_linked(maze[y][x], maze[y][x + 1]))
                rect_width += gap_size;

            sf::RectangleShape rect(sf::Vector2f(rect_width, rect_height));

            int mult = tile_size + gap_size;
            rect.setPosition(sf::Vector2f(mult * x, mult * y));

            window.draw(rect);
        }
    }
}

void Maze::play() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Maze");
    generate();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        draw(window);
        window.display();
    }
}