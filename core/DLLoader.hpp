/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** load .so
*/

#pragma once

#include <dlfcn.h>
#include <string>
#include <stdexcept>

template <typename T>
class DLLoader {
public:
    static T* load(const std::string& path, const std::string& entryPoint = "create");
};