LOCAL_PATH := $(call my-dir)
#cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1


include $(CLEAR_VARS)
LOCAL_MODULE    := lib_project
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_SRC_FILES := x32/liblib_project.a
endif

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
	LOCAL_SRC_FILES := x64/liblib_project.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := tolua
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_SRC_FILES := x32/libtolua.so
endif

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
	LOCAL_SRC_FILES := x64/libtolua.so
endif
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
SRC_DIR := ../../../source
LOCAL_MODULE := RenderingPlugin
LOCAL_C_INCLUDES += $(SRC_DIR)
LOCAL_LDLIBS := -llog
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES += $(SRC_DIR)/Logger/Logger.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/Threading/ThreadPool.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/Threading/ThreadManager.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/ResourceManager.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/RenderAPI.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/RenderingPlugin.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/core_api.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/LuaBinder/ToluaBinder.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/LuaBinder/LuaFunction.cpp
LOCAL_SRC_FILES += $(SRC_DIR)/LuaBinder/RustToLua.cpp

# OpenGL ES
LOCAL_SRC_FILES += $(SRC_DIR)/RenderAPI_OpenGLCoreES.cpp
LOCAL_LDLIBS += -lGLESv3
LOCAL_CPPFLAGS += -DSUPPORT_OPENGL_ES=1

# Vulkan (optional)
LOCAL_SRC_FILES += $(SRC_DIR)/RenderAPI_Vulkan.cpp
LOCAL_C_INCLUDES += $(NDK_ROOT)/sources/third_party/vulkan/src/include
LOCAL_CPPFLAGS += -DSUPPORT_VULKAN=1

LOCAL_STATIC_LIBRARIES := lib_project
LOCAL_SHARED_LIBRARIES := tolua
# build
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
#cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1