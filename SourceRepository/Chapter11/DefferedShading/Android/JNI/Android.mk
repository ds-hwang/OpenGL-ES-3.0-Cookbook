# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
#include $(LOCAL_PATH)/zlib/Android.mk
#LOCAL_PATH := $(LOCAL_PATH)

#include $(CLEAR_VARS)
FRAMEWORK_DIR = ../../../../../GLPIFramework
SCENE_DIR = ../../Scene
GLM_SRC_PATH = $(FRAMEWORK_DIR)/glm
ZLIB_DIR = $(FRAMEWORK_DIR)/zlib

LOCAL_MODULE    := glNative
#LOCAL_CFLAGS    := -Werror -O3 -mno-thumb
#LOCAL_CPPFLAGS   := -O3 -mno-thumb
LOCAL_C_INCLUDES := $(GLM_SRC_PATH)/core \
		$(GLM_SRC_PATH)/gtc \
		$(GLM_SRC_PATH)/gtx \
		$(GLM_SRC_PATH)/virtrev \
		$(ZLIB_DIR) \
		$(FRAMEWORK_DIR) \
		$(SCENE_DIR)

LOCAL_SRC_FILES := $(ZLIB_DIR)/adler32.c \
	$(ZLIB_DIR)/compress.c \
	$(ZLIB_DIR)/crc32.c \
	$(ZLIB_DIR)/deflate.c \
	$(ZLIB_DIR)/gzio.c \
	$(ZLIB_DIR)/infback.c \
	$(ZLIB_DIR)/inffast.c \
	$(ZLIB_DIR)/inflate.c \
	$(ZLIB_DIR)/inftrees.c \
	$(ZLIB_DIR)/ioapi.c \
	$(ZLIB_DIR)/trees.c \
	$(ZLIB_DIR)/uncompr.c \
	$(ZLIB_DIR)/unzip.c \
	$(ZLIB_DIR)/zutil.c \
	$(FRAMEWORK_DIR)/GLutils.cpp \
	$(FRAMEWORK_DIR)/Cache.cpp \
	$(FRAMEWORK_DIR)/ShaderManager.cpp \
	$(FRAMEWORK_DIR)/ProgramManager.cpp \
	$(FRAMEWORK_DIR)/Transform.cpp \
	$(SCENE_DIR)/Model.cpp \
	$(SCENE_DIR)/Renderer.cpp \
	$(SCENE_DIR)/Cube.cpp \
	$(SCENE_DIR)/Teapot.cpp \
	$(SCENE_DIR)/ObjLoader.cpp \
	$(SCENE_DIR)/Triangle.cpp \
	../../NativeTemplate.cpp

LOCAL_SHARED_LIBRARIES := zlib
LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)	