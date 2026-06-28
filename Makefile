ARCHS = arm64 arm64e
TARGET = iphone:clang:latest:14.0
INSTALL_TARGET_PROCESSES = Fortnite

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = FortniteCheat

FortniteCheat_FILES = Tweak.xm GUI.m
FortniteCheat_CFLAGS = -fobjc-arc
FortniteCheat_LIBRARIES = substrate

include $(THEOS_MAKE_PATH)/tweak.mk

after-install::
	@install_name_tool -change /Library/Frameworks/CydiaSubstrate.framework/CydiaSubstrate @rpath/CydiaSubstrate.framework/CydiaSubstrate $(THEOS_STAGING_DIR)/Library/MobileSubstrate/DynamicLibraries/FortniteCheat.dylib || true
