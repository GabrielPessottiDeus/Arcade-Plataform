#pragma once
#include "../../core/IGraphic.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Pour le texte

class SDL : public IGraphic {
public:
    SDL();
    ~SDL() override;

    void init() override;
    void close() override;
    bool pollEvent(InputEvent& event) override;
    void draw(int x, int y, char c) override;
    void clear() override;
    void display() override;

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    TTF_Font* _font;
    const int CELL_SIZE = 32;
    const int WIDTH = 25 * CELL_SIZE;
    const int HEIGHT = 25 * CELL_SIZE + 50; // +50px pour le score
};