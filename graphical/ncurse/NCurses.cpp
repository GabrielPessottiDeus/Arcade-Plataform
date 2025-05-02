#include "NCurses.hpp"
#include <stdexcept>
#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "../../core/DLLoader.hpp"
#include "../../core/IGame.hpp"
#include "../../core/IGraphic.hpp"
#include <unistd.h>  // pour sleep()

/*using namespace std;
namespace fs = std::filesystem;*/

NCurses::NCurses() : 
    _window(nullptr), 
    _isInitialized(false) 
{}

NCurses::~NCurses() {
    if (_isInitialized) close();
}

void NCurses::init() {
  
    _window = initscr();
    if (!_window) throw std::runtime_error("NCurses init failed");
    
    start_color(); // Active les couleurs
    curs_set(0);   // Cache le curseur
    nodelay(stdscr, TRUE); // Mode non-bloquant
    keypad(stdscr, TRUE);  // Active les touches spéciales
    noecho();      // Désactive l'écho
    cbreak();
    timeout(100);  // Délai d'attente en ms
    
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);  // 'W' → Bleu
    init_pair(2, COLOR_RED, COLOR_BLACK);   // 'O' → Rouge
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // 'H' → Vert
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);// 'S' → Jaune
    init_pair(5, COLOR_WHITE, COLOR_BLACK);

    _isInitialized = true;
}

// Fermeture propre
void NCurses::close() {
    if (_window) {
        printf("\033[?1003l\n");
        fflush(stdout);
        endwin();
        _window = nullptr;
    }
    _isInitialized = false;
}

// Récupération des événements
bool NCurses::pollEvent(InputEvent& event) {
    MEVENT ev;
    int ch = getch();
    if (ch == ERR) {
        event.pressed = false;
        return false;
    }

    // Conversion spéciale pour les touches directionnelles
    switch(ch) {
        case KEY_UP:     event.key = InputEvent::UP; break;
        case KEY_DOWN:   event.key = InputEvent::DOWN; break;
        case KEY_LEFT:   event.key = InputEvent::LEFT; break;
        case KEY_RIGHT:  event.key = InputEvent::RIGHT; break;
        case KEY_MOUSE:  
            if(getmouse(&ev) == OK) {
                if(ev.bstate & BUTTON3_CLICKED) {  
                    event.key = InputEvent::BUTTON2;
                    event.x = ev.x;
                    event.y = ev.y;
                } else if((ev.bstate & BUTTON1_CLICKED)) {  
                    event.key = InputEvent::BUTTON1;
                    event.x = ev.x;
                    event.y = ev.y;
                }
                
            }
            break;
        case 27:         event.key = InputEvent::ESCAPE; break;
        case ' ':        event.key = InputEvent::SPACE; break;
        default:
            if (ch == 27) { // Gestion des séquences escape (pour certains terminaux)
                nodelay(stdscr, TRUE);
                int next = getch();
                if (next == ERR) event.key = InputEvent::ESCAPE;
                nodelay(stdscr, FALSE);
            }
            else {
                event.key = InputEvent::UNKNOWN;
            }
    }
    
    event.pressed = true;
    return true;
}

// Dessin d'un caractère
void NCurses::draw(int x, int y, char c) {
    if (!_isInitialized) return;

    // Gestion des couleurs
    switch(c) {
        case '#': attron(COLOR_PAIR(1)); break;
        case 'O': attron(COLOR_PAIR(2)); break;
        case '+': attron(COLOR_PAIR(3)); break;
        case '*': attron(COLOR_PAIR(4)); break;
        case '"': attron(COLOR_PAIR(4)); break;
        case '0': attron(COLOR_PAIR(5)); break;
        case 'X': attron(COLOR_PAIR(2)); break;
        case '1': attron(COLOR_PAIR(3)); break;
        case '2': attron(COLOR_PAIR(4)); break;
        case '3': attron(COLOR_PAIR(2)); break;
        case '4': attron(COLOR_PAIR(2)); break;
    }

    mvaddch(y, x, c); // NCurses utilise (y, x) → inversion ici
    attroff(COLOR_PAIR(1)); // Réinitialisation
}

// Rafraîchissement de l'écran
void NCurses::display() {
    if (_isInitialized) refresh();
}

// Nettoyage de l'écran
void NCurses::clear() {
    if (_isInitialized) ::clear();
}

// Fonction factory pour le chargement dynamique
extern "C" {
    IGraphic* createGraphic() {
        return new NCurses();
    }
}