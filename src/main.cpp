#include <BNM.hpp>
#include <android/log.h>
#include <dlfcn.h>
#include <thread>
#include <chrono>

#include "Il2Cpp-Headers.hpp"
#include "Il2CppMethodNames.hpp"
#include "menu.h"

#define LOG_TAG "BNM_Spawner"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace BNM;

const char* ITEM_SPAWN_CLASS = "ItemSpawn";
const char* ITEM_SPAWN_IMAGE = "Assembly-CSharp";
const char* MOB_SPAWN_CLASS  = "ChickenSpawn";
const char* MOB_SPAWN_IMAGE  = "Assembly-CSharp";
const char* FALLBACK_SPAWNER = "AnchorSpawner";

Struct<Vector3> MakeVector3(float x, float y, float z) {
    Struct<Vector3> v = Class("Vector3", "UnityEngine").New<Vector3>();
    v.SetFieldValue("x", x);
    v.SetFieldValue("y", y);
    v.SetFieldValue("z", z);
    return v;
}

void LogClassMethods(Class cls) {
    if (!cls) return;
    LOGI("Methods in %s:", cls.GetName().c_str());
    for (auto& m : cls.GetMethods()) {
        LOGI("  %s", m.GetName().c_str());
    }
}

extern "C" void SpawnItem(const char* itemNameOrId, float x, float y, float z) {
    LOGI("[SpawnItem] %s @ (%.1f, %.1f, %.1f)", itemNameOrId, x, y, z);
    try {
        Class itemSpawnCls = Class(ITEM_SPAWN_CLASS, Image(ITEM_SPAWN_IMAGE));
        if (!itemSpawnCls) itemSpawnCls = Class(FALLBACK_SPAWNER, Image(ITEM_SPAWN_IMAGE));
        if (!itemSpawnCls) { LOGE("No spawner class found"); return; }

        LogClassMethods(itemSpawnCls);

        Method<void> spawnMethod = itemSpawnCls.GetMethod("SpawnItem");
        if (!spawnMethod) spawnMethod = itemSpawnCls.GetMethod("Spawn");
        if (!spawnMethod) spawnMethod = itemSpawnCls.GetMethod("CreateItem");

        if (spawnMethod) {
            spawnMethod(itemNameOrId, MakeVector3(x, y, z));
        } else {
            LOGI("No matching spawn method - update with your dump.cs");
        }
    } catch (...) {
        LOGE("SpawnItem crashed");
    }
}

extern "C" void SpawnMob(const char* mobType, float x, float y, float z) {
    LOGI("[SpawnMob] %s @ (%.1f, %.1f, %.1f)", mobType, x, y, z);
    try {
        Class mobCls = Class(MOB_SPAWN_CLASS, Image(MOB_SPAWN_IMAGE));
        if (!mobCls) mobCls = Class(FALLBACK_SPAWNER, Image(MOB_SPAWN_IMAGE));
        if (!mobCls) { LOGE("No mob class found"); return; }

        LogClassMethods(mobCls);

        Method<void> spawnMethod = mobCls.GetMethod("SpawnMob");
        if (!spawnMethod) spawnMethod = mobCls.GetMethod("Spawn");
        if (!spawnMethod) spawnMethod = mobCls.GetMethod("CreateMob");

        if (spawnMethod) {
            spawnMethod(mobType, MakeVector3(x, y, z));
        } else {
            LOGI("No matching spawn method - update with your dump.cs");
        }
    } catch (...) {
        LOGE("SpawnMob crashed");
    }
}

typedef bool (*eglSwapBuffers_t)(void* dpy, void* surface);
eglSwapBuffers_t orig_eglSwapBuffers = nullptr;

bool hook_eglSwapBuffers(void* dpy, void* surface) {
    if (Menu::showMenu) {
        Menu::Draw();
    }
    return orig_eglSwapBuffers ? orig_eglSwapBuffers(dpy, surface) : false;
}

void InstallRenderHook() {
    void* egl = dlopen("libEGL.so", RTLD_LAZY);
    if (egl) {
        void* addr = dlsym(egl, "eglSwapBuffers");
        if (addr) {
            LOGI("eglSwapBuffers at %p - hook this for ImGui", addr);
        }
    }
}

void InitMod() {
    LOGI("=== BNM + ImGui Spawner for Unity 6000.0.66f2 ===");
    BNM::Init();
    Menu::Init();
    InstallRenderHook();
    LOGI("Ready. Use the ImGui menu to spawn.");
}

extern "C" __attribute__((constructor))
void OnLoad() {
    std::thread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        InitMod();
    }).detach();
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    return JNI_VERSION_1_6;
}
