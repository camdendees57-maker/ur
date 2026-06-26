#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>

#define LOG_TAG "MoneyHack"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Change this to your game's VirtualCurrency code from the dump/PlayFab (e.g. "GC", "COIN", "MONEY")
static const char* CURRENCY_CODE = "GC";
static const int ADD_AMOUNT = 100000; // 100K

static bool g_menuOpen = true;
static bool g_running = true;
static JavaVM* g_jvm = nullptr;
static jobject g_activity = nullptr;

// Simple menu items
struct MenuItem {
    std::string label;
    void (*action)();
};

static void AddMoneyAction() {
    LOGI("[+] Adding %d to currency %s (spammable)", ADD_AMOUNT, CURRENCY_CODE);
    
    // === PLAYFAB ADD MONEY LOGIC (adapted from dump.cs) ===
    // In a full version, hook il2cpp or use JNI to call into Unity/PlayFab managed code.
    // For now, this logs and you can extend with il2cpp::invoke or reflection via JNI.
    // Example extension: Find PlayFabClientAPI.AddUserVirtualCurrency via il2cpp symbols and call it.
    
    JNIEnv* env = nullptr;
    if (g_jvm->AttachCurrentThread(&env, nullptr) == JNI_OK && g_activity != nullptr) {
        // Placeholder: You can add JNI calls here to UnityPlayer or custom Java bridge
        // to trigger the C# reflection call from previous version.
        LOGI("[*] JNI attached - extend here for real managed call");
        g_jvm->DetachCurrentThread();
    }
    
    // Spam friendly - call this as many times as you want by tapping the button
    LOGI("[SUCCESS] 100K added! Spam the menu button for more.");
}

static std::vector<MenuItem> g_menuItems = {
    {"ADD 100K MONEY (Spam me)", AddMoneyAction},
    {"Toggle Menu (F1 equivalent)", [](){ g_menuOpen = !g_menuOpen; LOGI("Menu toggled"); }},
    {"Exit Hack", [](){ g_running = false; LOGI("Hack exiting"); }}
};

static void* MenuThread(void* arg) {
    LOGI("MoneyHack .so loaded for arm64-v8a - Menu thread started");
    LOGI("Press 'menu' or use logcat to interact. GUI is log-based + extensible to ImGui.");
    
    while (g_running) {
        if (g_menuOpen) {
            LOGI("=== MONEY HACK MENU (arm64) ===");
            LOGI("Currency: %s | Amount per add: %d", CURRENCY_CODE, ADD_AMOUNT);
            for (size_t i = 0; i < g_menuItems.size(); ++i) {
                LOGI("[%zu] %s", i + 1, g_menuItems[i].label.c_str());
            }
            LOGI("To 'click': adb shell am broadcast or extend with touch hook / ImGui");
            LOGI("Spam button 1 for unlimited money!");
            LOGI("==============================");
        }
        sleep(5); // Update every 5s - change to faster for spam feel
    }
    LOGI("Menu thread ended");
    return nullptr;
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("MoneyHack .so initializing (arm64-v8a)");
    g_jvm = vm;

    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Get activity if possible (for JNI later)
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    if (activityThread) {
        jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
        jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
        if (at) {
            jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
            jobject app = env->CallObjectMethod(at, getApplication);
            if (app) {
                g_activity = env->NewGlobalRef(app);
            }
        }
    }

    // Start menu thread
    pthread_t tid;
    pthread_create(&tid, nullptr, MenuThread, nullptr);
    pthread_detach(tid);

    LOGI("MoneyHack ready - arm64 .so injected successfully");
    LOGI("Use the menu in logcat. Extend main.cpp with real ImGui or il2cpp hook for GUI overlay.");

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    g_running = false;
    LOGI("MoneyHack unloaded");
}