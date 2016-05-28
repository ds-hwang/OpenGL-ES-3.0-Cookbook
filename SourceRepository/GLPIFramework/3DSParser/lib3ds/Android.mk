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
	viewport.c \
	vector.c \
	tracks.c \
	tcb.c \
	shadow.c \
	quat.c \
	node.c \
	mesh.c \
	matrix.c \
	material.c \
	light.c \
	io.c \
	file.c \
	ease.c \
	chunk.c \
	camera.c \
	background.c \
	atmosphere.c

LOCAL_LDLIBS := -lz
LOCAL_CFLAGS := -I. -g

include $(BUILD_SHARED_LIBRARY)
