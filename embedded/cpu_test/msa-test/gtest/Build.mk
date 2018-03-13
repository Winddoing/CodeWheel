LOCAL_PATH := $(my-dir)

#######################################################################
# libc++
#
# #######################################################################
# # gtest lib host
#
include $(CLEAR_VARS)

LOCAL_MODULE := libgtest_host
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := src/gtest-all.cc \
					src/gtest_main.cc
LOCAL_CPP_INCLUDES:=include/
LOCAL_EXPORT_C_INCLUDE_DIRS := include/gtest

include $(BUILD_HOST_STATIC_LIBRARY)

# #######################################################################
# # gtest lib target
#
include $(CLEAR_VARS)

LOCAL_MODULE := libgtest_target
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := src/gtest-all.cc \
					src/gtest_main.cc
LOCAL_CPP_INCLUDES:= include/
LOCAL_EXPORT_C_INCLUDE_DIRS :=include/gtest

include $(BUILD_STATIC_LIBRARY)


