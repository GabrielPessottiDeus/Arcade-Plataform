/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** core
*/

#pragma once

#include "IGame.hpp"
#include "IGraphic.hpp"
#include <memory>

class Core {
private:
    std::unique_ptr<IGraphic> _graphic;
    std::unique_ptr<IGame> _game;
    
public:
    void loadGraphic(const std::string& path);
    void loadGame(const std::string& path);
    void showGameOverScreen(const std::string& lib, int put);
    void run(const std::string& lib);
};