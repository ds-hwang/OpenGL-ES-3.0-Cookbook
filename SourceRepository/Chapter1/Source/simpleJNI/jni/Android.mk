LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := jniNativeDev
LOCAL_SRC_FILES := HelloCookbookJNI.c

include $(BUILD_SHARED_LIBRARY)