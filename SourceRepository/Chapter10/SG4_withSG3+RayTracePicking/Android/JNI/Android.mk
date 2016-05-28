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
#LOCAL_PATH:= $(call my-dir)
MY_CUR_LOCAL_PATH := $(call my-dir)
FRAMEWORK_DIR 	= ../../../../GLPIFramework
SCENE_DIR 	= ../../Scene
GLM_SRC_PATH 	= $(FRAMEWORK_DIR)/glm
ZLIB_DIR 	= $(FRAMEWORK_DIR)/zlib
TEXTURE_LIB_PATH= $(FRAMEWORK_DIR)/ImageBuffer
WAVEFRONTOBJ_LIB_PATH=$(FRAMEWORK_DIR)/WaveFrontOBJ

include $(CLEAR_VARS)

include $(MY_CUR_LOCAL_PATH)/../../../../GLPIFramework/libpng/Android.mk \
	$(MY_CUR_LOCAL_PATH)/../../../../GLPIFramework/zlib/Android.mk
LOCAL_PATH := $(MY_CUR_LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := glNative
LOCAL_C_INCLUDES := $(GLM_SRC_PATH)/core \
		$(GLM_SRC_PATH)/gtc \
		$(GLM_SRC_PATH)/gtx \
		$(GLM_SRC_PATH)/virtrev \
		$(ZLIB_DIR) \
		$(FRAMEWORK_DIR) \
		$(FRAMEWORK_DIR)/Geometry \
		$(SCENE_DIR) \
		$(FREEIMAGELIB_DIR)/Source \
		$(TEXTURE_LIB_PATH) \
		$(WAVEFRONTOBJ_LIB_PATH)

LOCAL_SRC_FILES := $(FRAMEWORK_DIR)/GLutils.cpp \
	$(FRAMEWORK_DIR)/Cache.cpp \
	$(FRAMEWORK_DIR)/ShaderManager.cpp \
	$(FRAMEWORK_DIR)/ProgramManager.cpp \
	$(FRAMEWORK_DIR)/Transform.cpp \
	$(FRAMEWORK_DIR)/Light.cpp \
	$(FRAMEWORK_DIR)/Material.cpp \
	$(FRAMEWORK_DIR)/Geometry/Ray.cpp \
	$(SCENE_DIR)/RendererEx.cpp \
	$(SCENE_DIR)/Object.cpp \
	$(SCENE_DIR)/ModelEx.cpp \
	$(SCENE_DIR)/Scene.cpp \
	$(SCENE_DIR)/Event.cpp \
	$(TEXTURE_LIB_PATH)/Image.cpp \
	$(TEXTURE_LIB_PATH)/PngImage.cpp \
	$(WAVEFRONTOBJ_LIB_PATH)/WaveFrontOBJ.cpp \
	$(SCENE_DIR)/ObjLoader.cpp \
	../../NativeTemplate.cpp

LOCAL_SHARED_LIBRARIES := zlib GLPipng
LOCAL_LDLIBS    := -llog -lGLESv3
LOCAL_CFLAGS := -O3 -mno-thumb -Wno-write-strings

include $(BUILD_SHARED_LIBRARY)	
