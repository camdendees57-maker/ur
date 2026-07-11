#pragma once
#include <string>

namespace Menu {
    void Init();
    void Draw();
    void Shutdown();

    extern bool showMenu;
    extern std::string itemName;
    extern std::string mobType;
    extern float spawnPos[3];
}
