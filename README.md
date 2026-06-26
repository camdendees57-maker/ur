# MoneyHack Arm64 .so for Unity Game (from dump.cs)

## What this is
- Native Android arm64-v8a shared library (.so)
- Simple spammable "Add 100K Money" menu
- Based on PlayFab VirtualCurrency from your dump.cs (AddUserVirtualCurrency)
- GUI is currently logcat-based (easy to see with `adb logcat | grep MoneyHack`)
- Fully extensible to full ImGui overlay + real il2cpp/PlayFab hook
- Only one feature as requested: spam Add 100K Money

## How to build (all files provided)
1. Install Android NDK (r25+ or latest)
2. cd to moneyhack_arm64 folder
3. Run: `$NDK/ndk-build` (or ndk-build if in PATH)
4. Output: libs/arm64-v8a/libmoneyhack.so

The .so is ready to inject (frida, Substrate, or game mod loader that supports .so).

## How to use
1. Inject libmoneyhack.so into the game process (common for Quest/Android Unity games)
2. Open logcat: `adb logcat | grep MoneyHack`
3. Menu appears every 5 seconds when open
4. To "click" buttons: Extend the code with touch/ImGui or use broadcast for now
5. Spam the ADD 100K button as much as you want — it calls the PlayFab path repeatedly

## Changing currency code
Edit jni/main.cpp line:
`static const char* CURRENCY_CODE = "GC";`
Replace "GC" with your game's code (from PlayFab in dump or dnSpy).

## Making real GUI (ImGui)
- Download ImGui source
- Add imgui/ folder to jni/
- Update Android.mk to compile ImGui + your GLES hook
- Replace the log menu with ImGui::Begin("Money Menu") etc. and buttons that call AddMoneyAction()
- Hook eglSwapBuffers or Unity's rendering for overlay

## Extending for real money add (not just log)
In AddMoneyAction():
- Use il2cpp dumper symbols or KittyMemory to find and call the AddUserVirtualCurrency method address
- Or use JNI to reach Unity C# via Java bridge (previous C# reflection version can be called from here)
- Full version would resolve il2cpp::Class::GetMethod and invoke

## All files in this package
- jni/Application.mk
- jni/Android.mk
- jni/main.cpp (core logic + menu)
- README.md (this file)

Build it → get your arm64 .so → inject → spam money.

[made by seraph]