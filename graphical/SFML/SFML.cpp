#include "SFML.hpp"

SFML::SFML() : _window(sf::VideoMode(1920, 1080), "Arcade SFML") 
{
    _window.setVerticalSyncEnabled(true);
}

SFML::~SFML() {
    close();
}

void SFML::init() {
    if (!_font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Police arial.ttf manquante !");
    }
    _window.setFramerateLimit(30);
}

void SFML::close() {
    _window.close();
}

bool SFML::pollEvent(InputEvent& event) {
    sf::Event sfEvent;
    while (_window.pollEvent(sfEvent)) {
        if (sfEvent.type == sf::Event::Closed) {
            event.key = InputEvent::ESCAPE;
            event.pressed = true;
            return true;
        }
        if (sfEvent.type == sf::Event::KeyPressed) {
            switch(sfEvent.key.code) {
                case sf::Keyboard::Up:    event.key = InputEvent::UP; break;
                case sf::Keyboard::Down:  event.key = InputEvent::DOWN; break;
                case sf::Keyboard::Left:  event.key = InputEvent::LEFT; break;
                case sf::Keyboard::Right: event.key = InputEvent::RIGHT; break;
                case sf::Keyboard::Escape: event.key = InputEvent::ESCAPE; break;
                default: event.key = InputEvent::UNKNOWN;
            }
            event.pressed = true;
            return true;
        }
        if (sfEvent.type == sf::Event::MouseButtonPressed) {
            if (sfEvent.mouseButton.button == sf::Mouse::Left) {
                event.key = InputEvent::BUTTON1;
            } else if(sfEvent.mouseButton.button == sf::Mouse::Right) {
                event.key = InputEvent::BUTTON2;
            }

            event.x = sfEvent.mouseButton.x / CELL_SIZE;
            event.y = sfEvent.mouseButton.y / CELL_SIZE;
            event.pressed = true;
            return true;
        }
    }
    return false;
}

void SFML::draw(int x, int y, char c) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

    // Couleurs selon le caractère
    switch(c) {
        case '*': cell.setFillColor(sf::Color::Blue); break;  // Bordures
        case 'O': cell.setFillColor(sf::Color::Red); break;   // Nourriture
        case '#': cell.setFillColor(sf::Color::Green); break; // Tête
        case '+': cell.setFillColor(sf::Color::Yellow); break;// Corps
        case '"': cell.setFillColor(sf::Color::Blue); break;  // Bordures
        default:  // Score
            sf::Text text;
            text.setFont(_font);
            text.setString(std::string(1, c));
            text.setCharacterSize(20);
            text.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            _window.draw(text);
            return;
    }
     if (y >= 20) { // Zone dédiée au score (en dehors du terrain 20x20)
        sf::Text text;
        text.setFont(_font);
        text.setString(std::string(1, c));
        text.setCharacterSize(24);
        text.setPosition(x * CELL_SIZE, 20 * CELL_SIZE + 10); // 10px de marge
        text.setFillColor(sf::Color::White);
        _window.draw(text);
        return;
    }
    _window.draw(cell);
}

void SFML::clear() {
    _window.clear(sf::Color::Black);
}

void SFML::display() {
    _window.display();
}

extern "C" {
    IGraphic* createGraphic() {
        return new SFML();
    }
}