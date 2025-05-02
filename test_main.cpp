#include "core/Core.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2)
        return 1;
    Core core;
    core.run(argv[1]);
}