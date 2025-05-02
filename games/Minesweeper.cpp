#include "Minesweeper.hpp"
#include <vector>
#include <stdlib.h>

Minesweeper::Minesweeper() : _score(0), over(false) {
    map = {
        "###########",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "###########"
    };
    bombs.resize(9, std::vector<bool>(9, false));
    revealed.resize(9, std::vector<bool>(9, false));
    placeMines();
}

Minesweeper::~Minesweeper() {}

void Minesweeper::placeMines() {
    std::srand(std::time(nullptr));
    int placed = 0;
    while (placed < 10) {
        int x = std::rand() % 9;
        int y = std::rand() % 9;
        if (!bombs[x][y]) {
            bombs[x][y] = true;
            placed++;
        }
    }
}

int Minesweeper::countAdjacentMines(int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9 && bombs[nx][ny]) {
                count++;
            }
        }
    }
    return count;
}

void Minesweeper::reveal(int x, int y) {
    if (x < 0 || x >= 9 || y < 0 || y >= 9 || revealed[x][y]) return;
    revealed[x][y] = true;

    if (bombs[x][y]) {
        over = true;
        std::cout << "Game Over! You hit a mine.\n";
        return;
    }

    if (map [y + 1][x + 1] == '?')
        flags++;
    int adjacent = countAdjacentMines(x, y);
    map[y + 1][x + 1] = adjacent ? '0' + adjacent : ' ';
    _score += 10;

    if (adjacent == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0)
                    reveal(x + dx, y + dy);
            }
        }
    }
}

void Minesweeper::init() {
    _score = 0;
    over = false;
    map = {
        "###########",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "#         #",
        "###########"
    };
    bombs.assign(9, std::vector<bool>(9, false));
    revealed.assign(9, std::vector<bool>(9, false));
    placeMines();
}

void Minesweeper::update() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (revealed[x][y]) {
                int adjacent = countAdjacentMines(x, y);
                map[y + 1][x + 1] = adjacent ? '0' + adjacent : '0';
                if (bombs[x][y]) {
                    map[y + 1][x + 1] = 'X';
                }
            }
        }
    }

    if (_score == 710)
    {
        over = true;
        std::cout << "You win.\n";
        return;
    }
}

void Minesweeper::placeFlag(int x, int y)
{
    if (x < 0 || x >= 9 || y < 0 || y >= 9 || revealed[x][y] || flags == 0) return;
    if (map[y+1][x+1] == '?'){
        map[y+1][x+1] = ' ';
        flags++;
    } else if (map[y+1][x+1] == ' ') {
        map[y+1][x+1] = '?';
        flags--;
    }
}

void Minesweeper::handleInput(InputEvent& event) {
    if(event.key == InputEvent::BUTTON1)
    {
        reveal((event.x - 1), (event.y - 1));
    } else if(event.key == InputEvent::BUTTON2){
        placeFlag((event.x - 1), (event.y - 1));
    }

}

// void Minesweeper::handleMouseInput(int x, int y) {
//     reveal(x - 1, y - 1);
// }

int Minesweeper::getScore() const {
    return _score;
}

bool Minesweeper::isOver() const {
    return over;
}

const std::string& Minesweeper::getName() const {
    static std::string name = "Minesweeper";
    return name;
}

void Minesweeper::render(IGraphic* graphic) {
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 11; x++) {
            graphic->draw(x, y, map[y][x]);
        }
    }

    //score
    std::string str = std::to_string(_score);
    str = std::string(3 - str.size(), '0') + str; 
    int startX = 18;
    int y = 20;

    for (size_t i = 0; i < str.size(); ++i) {
        graphic->draw(startX + i, y, str[i]);
    }

    //flags
    str = std::to_string(flags);
    str = std::string(2 - str.size(), '0') + str; 
    startX = 19;
    y = 22;

    for (size_t i = 0; i < str.size(); ++i) {
        graphic->draw(startX + i, y, str[i]);
    }
}

extern "C" {
    IGame* create() {
        return new Minesweeper();
    }
}