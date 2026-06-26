LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := moneyhack
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_CFLAGS := -fvisibility=hidden
LOCAL_CPPFLAGS := -std=c++17 -fvisibility=hidden
include $(BUILD_SHARED_LIBRARY)