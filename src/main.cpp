#include <BNM.hpp>
#include <android/log.h>
#include <dlfcn.h>
#include <string>
#include <vector>

// Your provided headers (for symbol mapping / BNM compatibility)
#include "Il2Cpp-Headers.hpp"
#include "Il2CppMethodNames.hpp"

#define LOG_TAG "BNM_Spawner"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Using BNM namespace
using namespace BNM;

// ==================== CONFIG ====================
// Update these after dumping your game (dump.cs)
const char* ITEM_SPAWN_CLASS = "ItemSpawn";           // From your data.unity3d strings
const char* ITEM_SPAWN_IMAGE = "Assembly-CSharp";     // Most common

const char* MOB_SPAWN_CLASS = "ChickenSpawn";         // Example mob spawner from assets
const char* MOB_SPAWN_IMAGE = "Assembly-CSharp";

// Common fallback classes if above don't have direct spawn methods
const char* FALLBACK_SPAWNER = "AnchorSpawner";       // Another one from your strings

// ==================== HELPERS ====================
void LogClassMethods(Class cls) {
    if (!cls) return;
    LOGI("=== Methods in %s ===", cls.GetName().c_str());
    for (auto& m : cls.GetMethods()) {
        LOGI("  - %s", m.GetName().c_str());
    }
}

// Get Unity Vector3 (works in Unity 6 / 6000)
Struct<Vector3> MakeVector3(float x, float y, float z) {
    Struct<Vector3> v = Class("Vector3", "UnityEngine").New<Vector3>();
    v.SetFieldValue("x", x);
    v.SetFieldValue("y", y);
    v.SetFieldValue("z", z);
    return v;
}

// ==================== SPAWN FUNCTIONS ====================

// Spawn Item - targets your ItemSpawn class
extern "C" void SpawnItem(const char* itemNameOrId, float x, float y, float z) {
    LOGI("SpawnItem called: %s at (%.1f, %.1f, %.1f)", itemNameOrId, x, y, z);

    try {
        // Get the ItemSpawn class
        Class itemSpawnCls = Class(ITEM_SPAWN_CLASS, Image(ITEM_SPAWN_IMAGE));
        if (!itemSpawnCls) {
            LOGE("ItemSpawn class not found! Check your dump.cs");
            // Fallback
            itemSpawnCls = Class(FALLBACK_SPAWNER, Image(ITEM_SPAWN_IMAGE));
        }
        if (!itemSpawnCls) {
            LOGE("No suitable spawner class found");
            return;
        }

        LOGI("Found class: %s", itemSpawnCls.GetName().c_str());
        LogClassMethods(itemSpawnCls);

        // Try common spawn method names (update with your actual method from dump.cs)
        std::vector<std::string> possibleMethods = {
            "Spawn", "SpawnItem", "CreateItem", "AddItem", "DropItem", 
            "InstantiateItem", "GenerateItem"
        };

        Method<void> spawnMethod;
        for (const auto& methodName : possibleMethods) {
            spawnMethod = itemSpawnCls.GetMethod(methodName);
            if (spawnMethod) {
                LOGI("Using method: %s", methodName.c_str());
                break;
            }
        }

        if (spawnMethod) {
            // If the method takes parameters (item name + position), call it
            // Adjust parameter types based on your dump.cs
            spawnMethod(itemNameOrId, MakeVector3(x, y, z));   // Example signature
            LOGI("SpawnItem SUCCESS via direct method");
        } else {
            // Fallback: create object + set position (more advanced, needs your fields)
            LOGI("No direct spawn method found. Using generic Instantiate fallback...");
            
            // Example generic path - customize with your prefab/item ID system
            Class gameObjectCls = Class("GameObject", "UnityEngine");
            // You would normally load a prefab here or find existing item template
            // For now just log - implement full version with your ItemDatabase / prefab lookup
            LOGI("Generic spawn path ready - extend with your ItemDatabase / prefab logic");
        }
    } catch (const std::exception& e) {
        LOGE("SpawnItem exception: %s", e.what());
    }
}

// Spawn Mob - targets ChickenSpawn or your mob spawner
extern "C" void SpawnMob(const char* mobType, float x, float y, float z) {
    LOGI("SpawnMob called: %s at (%.1f, %.1f, %.1f)", mobType, x, y, z);

    try {
        Class mobCls = Class(MOB_SPAWN_CLASS, Image(MOB_SPAWN_IMAGE));
        if (!mobCls) {
            LOGE("ChickenSpawn / Mob class not found - check dump.cs");
            mobCls = Class(FALLBACK_SPAWNER, Image(MOB_SPAWN_IMAGE));
        }
        if (!mobCls) {
            LOGE("No mob spawner class found");
            return;
        }

        LOGI("Found mob class: %s", mobCls.GetName().c_str());
        LogClassMethods(mobCls);

        // Try common mob spawn methods
        std::vector<std::string> possibleMobMethods = {
            "Spawn", "SpawnMob", "CreateMob", "SpawnEnemy", "InstantiateMob",
            "ChickenSpawn", "SpawnChicken"
        };

        Method<void> spawnMethod;
        for (const auto& mName : possibleMobMethods) {
            spawnMethod = mobCls.GetMethod(mName);
            if (spawnMethod) {
                LOGI("Using mob method: %s", mName.c_str());
                break;
            }
        }

        if (spawnMethod) {
            spawnMethod(mobType, MakeVector3(x, y, z));
            LOGI("SpawnMob SUCCESS");
        } else {
            LOGI("Direct method not found. Add your exact method signature from dump.cs");
            // You can also do object_new + Invoke constructor or call a static factory
        }
    } catch (const std::exception& e) {
        LOGE("SpawnMob exception: %s", e.what());
    }
}

// ==================== INIT / ENTRY POINT ====================
extern "C" void InitMod() {
    LOGI("=== BNM Spawner Mod Initializing for Unity 6000.0.66f2 ===");

    // BNM auto-inits on first Class usage in modern versions, but explicit is safer
    BNM::Init();

    LOGI("BNM initialized successfully");
    LOGI("Ready to spawn mobs and items. Call SpawnItem / SpawnMob from Frida or your injector.");

    // Optional: Auto-spawn test on load (remove in production)
    // SpawnItem("TestItem", 0, 2, 0);
    // SpawnMob("Chicken", 5, 0, 5);
}

extern "C" __attribute__((constructor))
void OnLoad() {
    // Called automatically when .so is loaded
    LOGI("libBNM_Spawner.so loaded into process");
    InitMod();
}

// Also expose JNI style if needed for some loaders
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad called");
    return JNI_VERSION_1_6;
}
```

Now, the GitHub Actions workflow.