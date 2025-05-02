/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** core
*/

#include "Core.hpp"
#include "DLLoader.hpp"
#include "../graphical/ncurse/NCurses.hpp"
#include "IGraphic.hpp"
#include <unistd.h>  // pour sleep()
#include <stdexcept>
#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "IGame.hpp"
#include <SFML/Graphics.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace fs = std::filesystem;

void Core::loadGraphic(const std::string& path) {
    _graphic.reset(DLLoader<IGraphic>::load(path.c_str(), "createGraphic"));
}

void Core::loadGame(const std::string& path) {
    _game.reset(DLLoader<IGame>::load(path.c_str(), "create"));
}

std::vector<std::string> getFilesFromDir(const std::string& dirPath) {
    std::vector<std::string> files;
    try {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".so") {
                    files.push_back(filename);
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erro acessando " << dirPath << ": " << e.what() << std::endl;
    }
    return files;
}

/*void Core::showGameOverScreen(const std::string& lib, int input) {
    while(input != 'e') {
        if (lib == "ncurses") {
            clear();
            mvprintw(LINES / 2, (COLS - 10) / 2, "GAME OVER");
            mvprintw(LINES / 2 + 1, (COLS - 24) / 2, "Pressione qualquer tecla...");
            refresh();
            input = getch();
            endwin();
            if(input == 'r') {

            }
        } else if (lib == "sfml") {
            sf::RenderWindow window(sf::VideoMode(800, 600), "Game Over");
            sf::Font font;
            if (!font.loadFromFile("arial.ttf")) return;
                sf::Text text("GAME OVER", font, 50);
                text.setFillColor(sf::Color::Red);
                text.setPosition(200, 250);

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
                        window.close();
                }
                window.clear();
                window.draw(text);
                window.display();
                }
        } else if (lib == "sdl") {
            SDL_Init(SDL_INIT_VIDEO);
            TTF_Init();
            SDL_Window* window = SDL_CreateWindow("Game Over", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
            if (!font) return;
                SDL_Color color = {255, 0, 0};
                SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME OVER", color);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                SDL_Rect dst = {200, 250, 400, 100};

            bool quit = false;
            SDL_Event e;
            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN)
                        quit = true;
                }
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, &dst);
                SDL_RenderPresent(renderer);
            }

            SDL_DestroyTexture(texture);
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
        }
    }
}*/

void Core::run(const std::string& lib) {
    try {
        while (true) {
            IGraphic* graphic;
            IGame* game;
            int input, selected1, selected2;
            int altura = 10, largura = 20, y = 5, x1 = 5, x2 = x1 + largura + 5;

            auto menuOptionsLib = getFilesFromDir("lib/");
            auto menuOptionsGame = getFilesFromDir("gamesSo/");

            if (lib == "ncurses") {
                initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
                int countLib = menuOptionsLib.size();
                int countGame = menuOptionsGame.size();
                int maxOptionLengthLib = 0, maxOptionLengthGame = 0;
                for (auto& opt : menuOptionsLib) maxOptionLengthLib = std::max(maxOptionLengthLib, (int)opt.length());
                for (auto& opt : menuOptionsGame) maxOptionLengthGame = std::max(maxOptionLengthGame, (int)opt.length());

                input = getch();
                selected1 = 1;
                selected2 = 1;
                bool selection = true;

                while (input != 'q') {
                    clear();
                    WINDOW* quad1 = newwin(altura, largura, y, x1);
                    WINDOW* quad2 = newwin(altura, largura, y, x2);

                    box(quad1, 0, 0); box(quad2, 0, 0);
                    mvwprintw(quad1, 1, 3, "Lib Options");
                    for (int i = 0; i < countLib; i++)
                        mvwprintw(quad1, i + 2, 5, "%s", menuOptionsLib[i].c_str());

                    mvwprintw(quad2, 1, 3, "Games");
                    for (int i = 0; i < countGame; i++)
                        mvwprintw(quad2, i + 2, 5, "%s", menuOptionsGame[i].c_str());

                    if (selection && selected1 > 0 && selected1 <= countLib)
                        mvwchgat(quad1, selected1 + 1, 5, maxOptionLengthLib, A_STANDOUT, 1, NULL);
                    else if (!selection && selected2 > 0 && selected2 <= countGame)
                        mvwchgat(quad2, selected2 + 1, 5, maxOptionLengthGame, A_STANDOUT, 1, NULL);

                    refresh(); wrefresh(quad1); wrefresh(quad2);
                    input = getch();

                    if (selection) {
                        if (input == KEY_DOWN && selected1 < countLib) selected1++;
                        if (input == KEY_UP && selected1 > 0) selected1--;
                        if (input == 10) selection = false;
                    } else {
                        if (input == KEY_DOWN && selected2 < countGame) selected2++;
                        if (input == KEY_UP && selected2 > 0) selected2--;
                        if (input == 10) {
                            endwin();
                            graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1-1] , "createGraphic");
                            graphic->init();
                            game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2 - 1], "create");
                            game->init();
                            selected1--;
                            selected2--;
                            break;
                        }
                    }
                }
                if (input == 'q') {
                    endwin();
                    break;
                }
            } else if (lib == "sfml") {
                sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Menu");
                sf::Font font;
                if (!font.loadFromFile("arial.ttf")) {
                    std::cerr << "Erro ao carregar fonte 'arial.ttf'\n";
                    return;
                }

                selected1 = 0, selected2 = 0;
                bool selection = true;
                sf::Event event;

                while (window.isOpen()) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                        if (event.type == sf::Event::KeyPressed) {
                            if (selection) {
                                if (event.key.code == sf::Keyboard::Down && selected1 < (int)menuOptionsLib.size() - 1) selected1++;
                                if (event.key.code == sf::Keyboard::Up && selected1 > 0) selected1--;
                                if (event.key.code == sf::Keyboard::Enter) selection = false;
                            } else {
                                if (event.key.code == sf::Keyboard::Down && selected2 < (int)menuOptionsGame.size() - 1) selected2++;
                                if (event.key.code == sf::Keyboard::Up && selected2 > 0) selected2--;
                                if (event.key.code == sf::Keyboard::Enter) {
                                    window.close();
                                    graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1] , "createGraphic");
                                    graphic->init();
                                    game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2], "create");
                                    game->init();
                                    break;
                                }
                            }
                            if (event.key.code == sf::Keyboard::Q) window.close();
                        }
                    }

                    window.clear(sf::Color::Black);
                    sf::Text libTitle("Lib Options", font, 20); libTitle.setPosition(50, 30); window.draw(libTitle);
                    for (size_t i = 0; i < menuOptionsLib.size(); i++) {
                        sf::Text opt(menuOptionsLib[i], font, 18);
                        opt.setPosition(50, 60 + i * 25);
                        opt.setFillColor((selection && (int)i == selected1) ? sf::Color::Yellow : sf::Color::White);
                        window.draw(opt);
                    }

                    sf::Text gameTitle("Games", font, 20); gameTitle.setPosition(400, 30); window.draw(gameTitle);
                    for (size_t i = 0; i < menuOptionsGame.size(); i++) {
                        sf::Text opt(menuOptionsGame[i], font, 18);
                        opt.setPosition(400, 60 + i * 25);
                        opt.setFillColor((!selection && (int)i == selected2) ? sf::Color::Yellow : sf::Color::White);
                        window.draw(opt);
                    }

                    window.display();
                }     
                if (event.key.code == sf::Keyboard::Q) break;
            } else if (lib == "sdl2") {
                if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
                    std::cerr << "Erro ao inicializar SDL2: " << SDL_GetError() << "\n";
                    return;
                }
        
                SDL_Window* window = SDL_CreateWindow("SDL2 Menu",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
                SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        
                TTF_Font* font = TTF_OpenFont("arial.ttf", 20);
                if (!font) {
                    std::cerr << "Erro ao carregar fonte: " << TTF_GetError() << "\n";
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return;
                }
        
                selected1 = 0, selected2 = 0;
                bool selection = true;
                bool running = true;
        
                SDL_Event event;
                SDL_Keycode key;
                while (running) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
        
                    auto drawText = [&](const std::string& text, int x, int y, SDL_Color color) {
                        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_Rect dst = { x, y, surface->w, surface->h };
                        SDL_RenderCopy(renderer, texture, NULL, &dst);
                        SDL_FreeSurface(surface);
                        SDL_DestroyTexture(texture);
                    };
        
                    drawText("Lib Options", 50, 30, { 255, 255, 255, 255 });
                    for (size_t i = 0; i < menuOptionsLib.size(); ++i) {
                        SDL_Color color = (selection && (int)i == selected1) ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
                        drawText(menuOptionsLib[i], 50, 60 + (int)i * 25, color);
                    }
        
                    drawText("Games", 400, 30, { 255, 255, 255, 255 });
                    for (size_t i = 0; i < menuOptionsGame.size(); ++i) {
                        SDL_Color color = (!selection && (int)i == selected2) ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
                        drawText(menuOptionsGame[i], 400, 60 + (int)i * 25, color);
                    }
        
                    SDL_RenderPresent(renderer);

                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) running = false;
                        if (event.type == SDL_KEYDOWN) {
                            key = event.key.keysym.sym;
                            if (key == SDLK_q) {
                                TTF_CloseFont(font);
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                TTF_Quit();
                                SDL_Quit();
                                running = false;
                            }    
                            if (selection) {
                                if (key == SDLK_DOWN && selected1 < (int)menuOptionsLib.size() - 1) selected1++;
                                if (key == SDLK_UP && selected1 > 0) selected1--;
                                if (key == SDLK_RETURN) selection = false;
                            } else {
                                if (key == SDLK_DOWN && selected2 < (int)menuOptionsGame.size() - 1) selected2++;
                                if (key == SDLK_UP && selected2 > 0) selected2--;
                                if (key == SDLK_RETURN) {
                                    TTF_CloseFont(font);
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(window);
                                    TTF_Quit();
                                    SDL_Quit();
                                    graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1] , "createGraphic");
                                    graphic->init();
                                    game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2 ], "create");
                                    game->init();
                                    running = false;
                                }
                            }
                        }
                    }
        
                    
                }
                if (key == SDLK_q) {
                    break;
                }    
            }

            while (!game->isOver()) {
                InputEvent event;
                if (graphic->pollEvent(event)) {
                    game->handleInput(event);
                }
                game->update();
                graphic->clear();
                game->render(graphic);
                graphic->display();
                usleep(100000);
            }

            while(input != 'e') {
                if (lib == "ncurses") {
                    clear();
                    mvprintw(LINES / 2, (COLS - 10) / 2, "GAME OVER");
                    mvprintw(LINES / 2 + 1, (COLS - 24) / 2, "Press R to restart or E so quit");
                    refresh();
                    input = getch();
                    endwin();
                    if(input == 'r') {
                        ::clear();
                        endwin();
                        delete game;
                        graphic->close();
                        delete graphic;
                        graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1] , "createGraphic");
                        graphic->init();
                        game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2], "create");
                        game->init();
                        while (!game->isOver()) {
                            InputEvent event;
                            if (graphic->pollEvent(event)) {
                                game->handleInput(event);
                            }
                            game->update();
                            graphic->clear();
                            game->render(graphic);
                            graphic->display();
                            usleep(100000);
                        }
                    }
                } else if (lib == "sfml") {
                    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Over");
                    sf::Font font;
                    if (!font.loadFromFile("arial.ttf")) return;
                    
                    sf::Text gameOverText("GAME OVER", font, 50);
                    gameOverText.setFillColor(sf::Color::Red);
                    gameOverText.setPosition(200, 200);
                    
                    sf::Text infoText("Press R to restart or E to quit", font, 20);
                    infoText.setFillColor(sf::Color::White);
                    infoText.setPosition(100, 300);

                    input = '\0';

                    while (window.isOpen()) {
                        sf::Event event;
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed)
                                window.close();
                            if (event.type == sf::Event::KeyPressed) {
                                if (event.key.code == sf::Keyboard::R) {
                                    input = 'r';
                                    window.close();
                                }
                                else if (event.key.code == sf::Keyboard::E) {
                                    input = 'e';
                                    window.close();
                                    break;
                                }
                            }
                        }

                        window.clear();
                        window.draw(gameOverText);
                        window.draw(infoText);
                        window.display();
                    }

                    if (input == 'r') {
                        delete game;
                        graphic->close();
                        delete graphic;
                        graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1], "createGraphic");
                        graphic->init();
                        game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2], "create");
                        game->init();
                        while (!game->isOver()) {
                            InputEvent event;
                            if (graphic->pollEvent(event)) {
                                game->handleInput(event);
                            }
                            game->update();
                            graphic->clear();
                            game->render(graphic);
                            graphic->display();
                            usleep(100000);
                        }
                        input = '\0';
                    }
                }
                else if (lib == "sdl") {
                    SDL_Init(SDL_INIT_VIDEO);
                    TTF_Init();

                    SDL_Window* window = SDL_CreateWindow("Game Over", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
                    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                    TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
                    if (!font) return;

                    SDL_Color red = {255, 0, 0};
                    SDL_Color white = {255, 255, 255};

                    SDL_Surface* surfaceGameOver = TTF_RenderText_Solid(font, "GAME OVER", red);
                    SDL_Texture* textureGameOver = SDL_CreateTextureFromSurface(renderer, surfaceGameOver);
                    SDL_FreeSurface(surfaceGameOver);

                    SDL_Surface* surfaceInfo = TTF_RenderText_Solid(font, "Press R to restart or E to quit", white);
                    SDL_Texture* textureInfo = SDL_CreateTextureFromSurface(renderer, surfaceInfo);
                    SDL_FreeSurface(surfaceInfo);

                    SDL_Rect rectGameOver = {200, 200, 400, 100};
                    SDL_Rect rectInfo = {50, 350, 700, 50};

                    bool quit = false;
                    input = '\0';

                    SDL_Event e;
                    while (!quit) {
                        while (SDL_PollEvent(&e)) {
                            if (e.type == SDL_QUIT) {
                                input = 'm'; // Se fechar a janela, volta pro menu
                                quit = true;
                            } else if (e.type == SDL_KEYDOWN) {
                                SDL_Keycode key = e.key.keysym.sym;
                                if (key == SDLK_r) {
                                    input = 'r';
                                    quit = true;
                                } else if (key == SDLK_e) {
                                    input = 'e';
                                    quit = true;
                                }
                            }
                        }

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, textureGameOver, NULL, &rectGameOver);
                        SDL_RenderCopy(renderer, textureInfo, NULL, &rectInfo);
                        SDL_RenderPresent(renderer);
                    }

                    SDL_DestroyTexture(textureGameOver);
                    SDL_DestroyTexture(textureInfo);
                    TTF_CloseFont(font);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    TTF_Quit();
                    SDL_Quit();
                    if (input == 'r') {
                        // REINICIAR O JOGO
                        delete game;
                        graphic->close();
                        delete graphic;

                        graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1], "createGraphic");
                        graphic->init();

                        game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2], "create");
                        game->init();

                        while (!game->isOver()) {
                            InputEvent event;
                            if (graphic->pollEvent(event)) {
                                game->handleInput(event);
                            }
                            game->update();
                            graphic->clear();
                            game->render(graphic);
                            graphic->display();
                            usleep(100000);
                        }
                    }
                }

            }

            

            //showGameOverScreen(lib, input);

            /*while(input != 'e') {
                showGameOverScreen(lib);
                if(input == 'e') {
                    break;
                }
            }*/

            /*while(input != 'e') {
                WINDOW *quad3 = newwin(altura - 5, largura + 10, y, x2);
                box(quad3, 0, 0);
                int i = 0;
                mvwprintw(quad3, i + 2, 5, "Press R to restart game");
                i++;
                mvwprintw(quad3, i + 2, 5, "Press E to Quit");
                refresh();
                wrefresh(quad3);
                input = getch();

                if(input == 'r') {
                    ::clear();
                    endwin();
                    delete game;
                    graphic->close();
                    delete graphic;
                    graphic = DLLoader<IGraphic>::load("lib/" + menuOptionsLib[selected1] , "createGraphic");
                    graphic->init();
                    game = DLLoader<IGame>::load("gamesSo/" + menuOptionsGame[selected2], "create");
                    game->init();
                    while (!game->isOver()) {
                        InputEvent event;
                        if (graphic->pollEvent(event)) {
                            game->handleInput(event);
                        }
                        game->update();
                        graphic->clear();
                        game->render(graphic);
                        graphic->display();
                        usleep(100000);
                    }
                }

                }
            }*/
            delete game;
            graphic->close();
            delete graphic;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return;
    }
    return;
}
