/*
** EPITECH PROJECT, 2025
** game interface
** File description:
** game interface
*/

#pragma once
#include <string>
#include "IGraphic.hpp" // Ajout nécessaire

class IGame {
public:
    virtual ~IGame() = default;
    
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void handleInput(InputEvent& event) = 0; // STRUCT INSTEAD OF KEY
    virtual void render(IGraphic* graphic) = 0; // Ajouté ici
    virtual int getScore() const = 0;
    virtual bool isOver() const = 0;
    virtual const std::string& getName() const = 0;
};