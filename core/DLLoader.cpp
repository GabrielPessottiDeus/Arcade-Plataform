/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** load .so
*/

#include "DLLoader.hpp"
#include "IGraphic.hpp"
#include "IGame.hpp"

template <typename T>
T* DLLoader<T>::load(const std::string& path, const std::string& entryPoint)
{
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle)
        throw std::runtime_error("dlopen: " + std::string(dlerror()));

    void* symbol = dlsym(handle, entryPoint.c_str());
    if (!symbol) {
        dlclose(handle);
        throw std::runtime_error("dlsym: " + std::string(dlerror()));
    }

    typedef T* (*CreateFunc)();
    CreateFunc create = reinterpret_cast<CreateFunc>(symbol);
    return create();
}

// Instanciations explicites pour éviter les erreurs de linkage
template class DLLoader<class IGame>;
template class DLLoader<class IGraphic>;