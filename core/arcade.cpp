/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** main
*/

#include "../core/Core.hpp"
#include <iostream>

int main() {
    Core core;

    try {
        // Charger une lib graphique et un jeu (chemins relatifs/absolus)
        core.loadGraphic("./lib/libncurse.so");  // Adapte le chemin selon ton Makefile
        //core.loadGame("./games/libsnake.so");
        
        // Lancer la boucle principale
        core.run();
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}