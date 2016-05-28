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

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := mylib3ds
LOCAL_SRC_FILES := \
	lib3ds/viewport.c \
	lib3ds/vector.c \
	lib3ds/tracks.c \
	lib3ds/tcb.c \
	lib3ds/shadow.c \
	lib3ds/quat.c \
	lib3ds/node.c \
	lib3ds/mesh.c \
	lib3ds/matrix.c \
	lib3ds/material.c \
	lib3ds/light.c \
	lib3ds/io.c \
	lib3ds/file.c \
	lib3ds/ease.c \
	lib3ds/chunk.c \
	lib3ds/camera.c \
	lib3ds/background.c \
	lib3ds/atmosphere.c

LOCAL_LDLIBS := -lz
LOCAL_CFLAGS := -I. -g

include $(BUILD_SHARED_LIBRARY)
