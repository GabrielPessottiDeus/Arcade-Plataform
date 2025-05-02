#include "SDL.hpp"
#include <stdexcept>

SDL::SDL() : _window(nullptr), _renderer(nullptr), _font(nullptr) {}

SDL::~SDL() {
    if (_font) TTF_CloseFont(_font); // <-- Ajouter cette ligne
    if (_renderer) SDL_DestroyRenderer(_renderer);
    if (_window) SDL_DestroyWindow(_window);
    TTF_Quit();
    SDL_Quit();
}

void SDL::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL init failed");
    }

    _window = SDL_CreateWindow(
        "Arcade SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
    _font = TTF_OpenFont("arial.ttf", 24);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
}

void SDL::close() {
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr; // Évite les double-free
    }
    if(_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if(_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    TTF_Quit();
    SDL_Quit();
}

bool SDL::pollEvent(InputEvent& event) {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        if (sdlEvent.type == SDL_QUIT) {
            event.key = InputEvent::ESCAPE;
            event.pressed = true;
            return true;
        }
        if (sdlEvent.type == SDL_KEYDOWN) {
            switch(sdlEvent.key.keysym.sym) {
                case SDLK_UP:     event.key = InputEvent::UP; break;
                case SDLK_DOWN:   event.key = InputEvent::DOWN; break;
                case SDLK_LEFT:   event.key = InputEvent::LEFT; break;
                case SDLK_RIGHT:  event.key = InputEvent::RIGHT; break;
                case SDLK_ESCAPE: event.key = InputEvent::ESCAPE; break;
                default:          event.key = InputEvent::UNKNOWN;
            } 
            event.pressed = true;
            return true;
        } 

        if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
            if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                event.key = InputEvent::BUTTON1;
            } else if(sdlEvent.button.button == SDL_BUTTON_RIGHT){
                event.key = InputEvent::BUTTON2;
            }

            event.x = sdlEvent.button.x / CELL_SIZE;
            event.y = sdlEvent.button.y / CELL_SIZE;
            event.pressed = true;
            return true;
        }
    }
    return false;
}

void SDL::draw(int x, int y, char c) {
    SDL_Rect rect = {
        x * CELL_SIZE,
        y * CELL_SIZE,
        CELL_SIZE - 2,
        CELL_SIZE - 2
    };

    switch(c) {
        case '*': SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255); break; // Bordures
        case 'O': SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255); break; // Nourriture
        case '#': SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255); break; // Tête
        case '+': SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255); break; // Corps
        case '"': SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255); break; // Bordures
        default: // Score
            SDL_Color color = {255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(_font, std::string(1, c).c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
            SDL_Rect textRect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(_renderer, texture, nullptr, &textRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            return;
    }

    SDL_RenderFillRect(_renderer, &rect);
}

void SDL::clear() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void SDL::display() {
    SDL_RenderPresent(_renderer);
}

extern "C" {
    IGraphic* createGraphic() {
        return new SDL();
    }
}