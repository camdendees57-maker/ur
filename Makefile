ARCHS = arm64
TARGET = iphone:clang:latest:14.0
INSTALL_TARGET_PROCESSES = Fortnite

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = FortniteCheat

FortniteCheat_FILES = Tweak.xm
FortniteCheat_CFLAGS = -fobjc-arc
FortniteCheat_LIBRARIES = substrate

include $(THEOS_MAKE_PATH)/tweak.mk
