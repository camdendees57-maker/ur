#include "menu.h"
#include "imgui.h"           // Will be available after adding ImGui submodule
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "BNM_Menu", __VA_ARGS__)

// Forward declarations from main.cpp
extern "C" void SpawnItem(const char* itemNameOrId, float x, float y, float z);
extern "C" void SpawnMob(const char* mobType, float x, float y, float z);

namespace Menu {

bool showMenu = true;
std::string itemName = "Sword";
std::string mobType = "Chicken";
float spawnPos[3] = {0.0f, 2.0f, 0.0f};

void Init() {
    LOGI("ImGui Menu initialized");
    // TODO: Full ImGui context + style + font setup here
    // After adding ImGui submodule + backend (imgui_impl_opengl3 + input)
}

void Draw() {
    if (!showMenu) return;

    // Example: Toggle with a controller button or Frida bool flip
    ImGui::Begin("BNM Spawner - Unity 6000.0.66f2", &showMenu, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Target: Unity 6000.0.66f2 | BNM + ImGui");
    ImGui::Separator();

    // ========== ITEM SPAWNER ==========
    if (ImGui::CollapsingHeader("Item Spawner", ImGuiTreeNodeFlags_DefaultOpen)) {
        static char itemBuf[128] = "Sword";
        ImGui::InputText("Item Name / ID", itemBuf, IM_ARRAYSIZE(itemBuf));
        itemName = itemBuf;

        ImGui::DragFloat3("Position (X Y Z)", spawnPos, 0.5f);

        if (ImGui::Button("Spawn Item", ImVec2(120, 40))) {
            SpawnItem(itemName.c_str(), spawnPos[0], spawnPos[1], spawnPos[2]);
            LOGI("Spawned item from menu: %s", itemName.c_str());
        }

        ImGui::SameLine();
        if (ImGui::Button("Spawn at Player", ImVec2(140, 40))) {
            // TODO: Get player position via BNM (Class("Player") or Camera.main.transform.position)
            SpawnItem(itemName.c_str(), spawnPos[0], spawnPos[1] + 1.0f, spawnPos[2]);
        }
    }

    // ========== MOB SPAWNER ==========
    if (ImGui::CollapsingHeader("Mob Spawner", ImGuiTreeNodeFlags_DefaultOpen)) {
        static char mobBuf[128] = "Chicken";
        ImGui::InputText("Mob Type", mobBuf, IM_ARRAYSIZE(mobBuf));
        mobType = mobBuf;

        ImGui::DragFloat3("Position (X Y Z)", spawnPos, 0.5f);  // shared pos for simplicity

        if (ImGui::Button("Spawn Mob", ImVec2(120, 40))) {
            SpawnMob(mobType.c_str(), spawnPos[0], spawnPos[1], spawnPos[2]);
            LOGI("Spawned mob from menu: %s", mobType.c_str());
        }
    }

    ImGui::Separator();
    ImGui::Text("Tip: Use your dump.cs to add more items/mobs to presets");

    // Quick presets (customize with your game items)
    if (ImGui::CollapsingHeader("Quick Presets")) {
        if (ImGui::Button("Spawn Test Item")) { 
            SpawnItem("TestItem", 0, 3, 0); 
        }
        ImGui::SameLine();
        if (ImGui::Button("Spawn Chicken")) { 
            SpawnMob("Chicken", 5, 0, 5); 
        }
    }

    ImGui::End();

    // Optional second window for logs / status
    ImGui::Begin("Spawner Log");
    ImGui::Text("Last action logged in logcat (tag: BNM_Spawner)");
    ImGui::End();
}

void Shutdown() {
    LOGI("Menu shutdown");
}

} // namespace Menu
```

Now, heavily update main.cpp to integrate the menu and provide a render hook example.