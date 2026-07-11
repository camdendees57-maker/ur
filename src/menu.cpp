#include "menu.h"
#include "imgui.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "BNM_Menu", __VA_ARGS__)

extern "C" void SpawnItem(const char* itemNameOrId, float x, float y, float z);
extern "C" void SpawnMob(const char* mobType, float x, float y, float z);

namespace Menu {

bool showMenu = true;
std::string itemName = "Sword";
std::string mobType = "Chicken";
float spawnPos[3] = {0.0f, 2.0f, 0.0f};

void Init() {
    LOGI("ImGui Menu initialized");
}

void Draw() {
    if (!showMenu) return;

    ImGui::Begin("BNM Spawner - Unity 6000", &showMenu, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Item Spawner", ImGuiTreeNodeFlags_DefaultOpen)) {
        static char itemBuf[128] = "Sword";
        ImGui::InputText("Item Name/ID", itemBuf, IM_ARRAYSIZE(itemBuf));
        itemName = itemBuf;

        ImGui::DragFloat3("Position", spawnPos, 0.5f);

        if (ImGui::Button("Spawn Item", ImVec2(140, 45))) {
            SpawnItem(itemName.c_str(), spawnPos[0], spawnPos[1], spawnPos[2]);
        }
    }

    if (ImGui::CollapsingHeader("Mob Spawner", ImGuiTreeNodeFlags_DefaultOpen)) {
        static char mobBuf[128] = "Chicken";
        ImGui::InputText("Mob Type", mobBuf, IM_ARRAYSIZE(mobBuf));
        mobType = mobBuf;

        ImGui::DragFloat3("Position", spawnPos, 0.5f);

        if (ImGui::Button("Spawn Mob", ImVec2(140, 45))) {
            SpawnMob(mobType.c_str(), spawnPos[0], spawnPos[1], spawnPos[2]);
        }
    }

    if (ImGui::CollapsingHeader("Quick Presets")) {
        if (ImGui::Button("Spawn Test Item")) SpawnItem("TestItem", 0, 3, 0);
        ImGui::SameLine();
        if (ImGui::Button("Spawn Chicken")) SpawnMob("Chicken", 5, 0, 5);
    }

    ImGui::End();
}

void Shutdown() {}
}
