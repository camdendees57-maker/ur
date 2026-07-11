#pragma once
#include <string>
#include <vector>

namespace Menu {
    void Init();
    void Draw();
    void Shutdown();

    // Shared state with spawner
    extern bool showMenu;
    extern std::string itemName;
    extern std::string mobType;
    extern float spawnPos[3];
}
```

Now the menu implementation (basic ImGui skeleton - user will need full backend integration).