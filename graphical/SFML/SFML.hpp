#pragma once
#include "../../core/IGraphic.hpp"
#include <SFML/Graphics.hpp>

class SFML : public IGraphic {
public:
    SFML();
    ~SFML() override;

    void init() override;
    void close() override;
    bool pollEvent(InputEvent& event) override;
    void draw(int x, int y, char c) override;
    void clear() override;
    void display() override;

private:
    sf::RenderWindow _window;
    sf::Font _font;
    const int CELL_SIZE = 40; // Taille d'une cellule en pixel
};
