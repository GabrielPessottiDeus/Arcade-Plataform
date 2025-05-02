#pragma once

#include "../core/IGame.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

/*
----- OK -------
9x9 map -> init map
10 -> place bombs randomly
manage mouse inputs
*/

// place flags!
// timer !!!!!!!!!!
// first cell check!!



class Minesweeper : public IGame {
private:
    int _score;
    std::vector<std::string> map;
    std::vector<std::vector<bool>> bombs;
    std::vector<std::vector<bool>> revealed;
    int flags = 10;
    bool over;
    void placeMines();
    int countAdjacentMines(int x, int y);
    void reveal(int x, int y);
    
public:
    Minesweeper();
    ~Minesweeper();
    void init() override;
    void update() override;
    void handleInput(InputEvent& event) override;
    // void handleMouseInput(int x, int y) override;
    void render (IGraphic* graphic) override;
    int getScore() const override;
    bool isOver() const override;
    const std::string& getName() const override;
    void placeFlag(int x, int y);
    // void displayBoard() const;
};
