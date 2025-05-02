#pragma once
#include "../core/IGame.hpp"
#include <vector>

class Snake : public IGame {
public:
    Snake();
    void init() override;
    void update() override;
    void handleInput(InputEvent& event) override; //!!
    void render (IGraphic* graphic) override;
    int getScore() const override;
    bool isOver() const override;
    const std::string& getName() const override;

private:
    enum Direction { UP, DOWN, LEFT, RIGHT };
    
    struct Segment {
        int x, y;
        Segment(int x, int y) : x(x), y(y) {}
    };

    std::vector<Segment> _body;
    Direction _dir;
    int _score;
    bool _gameOver;
    Segment _food;
    
    void spawnFood();
    bool checkCollision();
};