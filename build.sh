#!/bin/bash
# Build script for MoneyHack arm64 .so
# Requires Android NDK in PATH or set NDK variable

NDK=${NDK:-$HOME/Android/Sdk/ndk-bundle}
if [ ! -d "$NDK" ]; then
    echo "Set NDK path or install Android NDK"
    exit 1
fi

echo "Building arm64-v8a MoneyHack .so..."
$NDK/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk APP_ABI=arm64-v8a

echo "Done! Check libs/arm64-v8a/libmoneyhack.so"
ls -l libs/arm64-v8a/ 2>/dev/null || echo "Build output in obj/local/ or libs/"