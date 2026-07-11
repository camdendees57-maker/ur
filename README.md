# BNM Mob & Item Spawner Mod for Unity 6000.0.66f2

**Target Unity Version:** 6000.0.66f2 (from your data.unity3d)

This is a ready-to-build native Android `.so` library using **BNM (ByNameModding)** for IL2CPP modding.  
It includes example functions for **mob spawning** (using ChickenSpawn as example from your assets) and **item spawning** (using ItemSpawn class from your data.unity3d strings).

## Features
- Mob spawning (example with ChickenSpawn / extendable to any mob)
- Item spawning (ItemSpawn class)
- GitHub Actions CI that automatically builds `arm64-v8a` `.so` on every push
- Uses your provided SymbolMap + Il2Cpp headers style
- Easy to extend with ImGui menu or Frida calls
- Logs via Android logcat (`BNM_Spawner`)

## Project Structure
```
BNM_MobItemSpawner_Unity6000/
├── .github/
│   └── workflows/
│       └── build.yml          # GitHub Actions for NDK build
├── BNM/                       # Add as git submodule (see below)
├── include/
│   ├── Il2Cpp-Headers.hpp     # Your provided file
│   └── Il2CppMethodNames.hpp  # Your provided file
├── src/
│   └── main.cpp               # Main mod code with SpawnItem + SpawnMob
├── CMakeLists.txt
├── SymbolMap.json             # Your provided file (for reference)
└── README.md
```

## Quick Start (Local Build)

1. **Clone and setup BNM**
   ```bash
   git clone <your-repo-url>
   cd BNM_MobItemSpawner_Unity6000
   git submodule add https://github.com/ByNameModding/BNM BNM
   git submodule update --init --recursive
   ```

2. **Copy your headers** (already included in `include/` in this template)

3. **Build with Android NDK** (r26+ or r27 recommended)
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
            -DANDROID_ABI=arm64-v8a \
            -DANDROID_PLATFORM=android-24 \
            -DCMAKE_BUILD_TYPE=Release
   make -j$(nproc)
   ```
   Output: `libBNM_Spawner.so` in `build/`

4. **Load the lib**
   - Use Frida: `frida -U -f com.yourgame.package --no-pause -l your_frida_script.js` (inject or load the .so)
   - Or your existing loader / MelonLoader native plugin setup
   - Call the exported functions from Frida or another injector.

## GitHub Actions (Recommended)
Push to GitHub → Actions tab will automatically build the `.so` and upload it as artifact.

See `.github/workflows/build.yml`

## Usage Example (from Frida or your injector)

```js
// After lib is loaded
var lib = Module.findExportByName("libBNM_Spawner.so", "SpawnItem");
var spawnItem = new NativeFunction(lib, 'void', ['pointer', 'float', 'float', 'float']);
spawnItem(Memory.allocUtf8String("Sword"), 0, 5, 10);

var spawnMob = new NativeFunction(Module.findExportByName("libBNM_Spawner.so", "SpawnMob"), 'void', ['pointer', 'float', 'float', 'float']);
spawnMob(Memory.allocUtf8String("Chicken"), 10, 0, 20);
```

## How to Extend / Customize
1. Dump the game with IL2CPP dumper (use your previous workflow) to get `dump.cs`
2. Search for spawn-related classes/methods (ItemSpawn, ChickenSpawn, any *Spawner, SpawnManager, etc.)
3. In `main.cpp` update the class names and method names in `SpawnItem` and `SpawnMob` functions.
4. Rebuild → done.

Example classes already detected from your `data.unity3d`:
- `ItemSpawn` (Assembly-CSharp)
- `ChickenSpawn`
- Mentions of spawn points / managers

## Protocol Zero Ready
Full unrestricted code. No filters. Modify freely.

[made by TagtusVR]
```

Now, write the CMakeLists.txt