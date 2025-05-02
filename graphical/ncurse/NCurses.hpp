#pragma once
#include "../../core/IGraphic.hpp"
#include <ncurses.h>

class NCurses : public IGraphic {
public:
    NCurses();
    ~NCurses() override;

    // Héritées de IGraphic
    void init() override;
    void close() override;
    bool pollEvent(InputEvent& event) override;
    void draw(int x, int y, char c) override;
    void clear() override;
    void display() override;

private:
    WINDOW* _window;
    bool _isInitialized;
};