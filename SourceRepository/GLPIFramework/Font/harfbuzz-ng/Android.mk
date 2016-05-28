LOCAL_PATH:= $(call my-dir)

#############################################################
#   Android make file for building the harfbuzz library
#############################################################

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:= \
	src/hb-blob.cc \
	src/hb-buffer-serialize.cc \
	src/hb-buffer.cc \
	src/hb-common.cc \
	src/hb-fallback-shape.cc \
	src/hb-font.cc \
	src/hb-ft.cc \
	src/hb-ot-tag.cc \
	src/hb-set.cc \
	src/hb-shape.cc \
	src/hb-shape-plan.cc \
	src/hb-shaper.cc \
	src/hb-tt-font.cc \
	src/hb-unicode.cc \
	src/hb-warning.cc \
	src/hb-ot-layout.cc \
	src/hb-ot-map.cc \
	src/hb-ot-shape.cc \
	src/hb-ot-shape-complex-arabic.cc \
	src/hb-ot-shape-complex-default.cc \
	src/hb-ot-shape-complex-indic.cc \
	src/hb-ot-shape-complex-indic-table.cc \
	src/hb-ot-shape-complex-myanmar.cc \
	src/hb-ot-shape-complex-sea.cc \
	src/hb-ot-shape-complex-thai.cc \
	src/hb-ot-shape-normalize.cc \
	src/hb-ot-shape-fallback.cc \
	#src/hb-icu.cc

LOCAL_CPP_EXTENSION := .cc

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/src \
        external/icu4c/common \
				$(LOCAL_PATH)/src $(LOCAL_PATH)/../freetype/include

LOCAL_CFLAGS := -DHAVE_OT

LOCAL_MODULE:= libGLPiharfbuzz

LOCAL_STATIC_LIBRARIES := GLPift2

include $(BUILD_SHARED_LIBRARY)


