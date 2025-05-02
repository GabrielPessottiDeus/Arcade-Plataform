#include "Snake.hpp"
#include <cstdlib>
#include <ctime>

Snake::Snake() : 
    _dir(RIGHT),
    _score(0),
    _gameOver(false),
    _food(0, 0) 
{
    srand(time(nullptr));
    _body.emplace_back(10, 10); // Tête
    _body.emplace_back(9, 10);  // Corps
    spawnFood();
}

void Snake::init() {}

void Snake::update() {
    if (_gameOver) return;

    // Déplacement du corps
    for (size_t i = _body.size() - 1; i > 0; --i) {
        _body[i] = _body[i - 1];
    }

    // Déplacement de la tête
    auto& head = _body[0];
    switch(_dir) {
        case UP:    head.y--; break;
        case DOWN:  head.y++; break;
        case LEFT:  head.x--; break;
        case RIGHT: head.x++; break;
    }

    // Collisions
    if (checkCollision()) {
        _gameOver = true;
        return;
    }

    // Manger la nourriture
    if (head.x == _food.x && head.y == _food.y) {
        _body.push_back(_body.back());
        _score += 10;
        spawnFood();
    }
}

void Snake::handleInput(InputEvent& event) {
    int key = event.key;
    switch(key) {
        case InputEvent::UP:    if (_dir != DOWN)  _dir = UP; break;
        case InputEvent::DOWN:  if (_dir != UP)    _dir = DOWN; break;
        case InputEvent::LEFT:  if (_dir != RIGHT) _dir = LEFT; break;
        case InputEvent::RIGHT: if (_dir != LEFT)  _dir = RIGHT; break;
    }
}

void Snake::render(IGraphic* graphic) {
    // Bordures avec le caractère 'W' pour SFML/NCurses
    for (int x = 0; x < 20; x++) {
        graphic->draw(x, 0, '*');    // Bordure haute
        graphic->draw(x, 19, '*');   // Bordure basse
    }
    for (int y = 0; y < 20; y++) {
        graphic->draw(0, y, '"');    // Bordure gauche
        graphic->draw(19, y, '"');   // Bordure droite
    }

    // Nourriture
    graphic->draw(_food.x, _food.y, 'O');

    // Serpent
    for (size_t i = 0; i < _body.size(); ++i) {
        char c = (i == 0) ? '#' : '+'; // 'H'=Tête, 'S'=Corps
        graphic->draw(_body[i].x, _body[i].y, c);
    }

    // Score
    std::string scoreText = "Score:" + std::to_string(_score);
    for (size_t i = 0; i < scoreText.size(); ++i) {
        graphic->draw(i + 1, 20, scoreText[i]); // y=20 → en dessous du terrain
    }
}

int Snake::getScore() const { return _score; }
bool Snake::isOver() const { return _gameOver; }
const std::string& Snake::getName() const { static std::string name = "snake"; return name; }

// Méthodes privées
void Snake::spawnFood() {
    // Générer entre 1 et 18 (évite les murs en 0 et 19)
    _food.x = (rand() % 18) + 1;
    _food.y = (rand() % 18) + 1;

    // Vérifier que la nourriture n'apparaît pas sur le serpent
    for (const auto& segment : _body) {
        if (_food.x == segment.x && _food.y == segment.y) {
            spawnFood(); // Régénérer si collision avec le serpent
            return;
        }
    }
}

bool Snake::checkCollision() {
    const auto& head = _body[0];
    
    // Collision avec les murs (x=0, x=19, y=0, y=19)
    if (head.x < 1 || head.x >= 19 || head.y < 1 || head.y >= 19) {
        return true;
    }
    
    // Auto-collision
    for (size_t i = 1; i < _body.size(); ++i) {
        if (head.x == _body[i].x && head.y == _body[i].y) {
            return true;
        }
    }
    return false;
}


extern "C" {
    IGame* create() {
        return new Snake();
    }
}