LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
ZLIB_DIR = zlib
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := adler32.c \
	compress.c \
	crc32.c \
	deflate.c \
	gzio.c \
	infback.c \
	inffast.c \
	inflate.c \
	inftrees.c \
	ioapi.c \
	trees.c \
	uncompr.c \
	unzip.c \
	zutil.c
include $(BUILD_SHARED_LIBRARY)
