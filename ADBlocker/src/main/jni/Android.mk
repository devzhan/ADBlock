LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := adblockplus
LOCAL_SRC_FILES := $(LOCAL_PATH)/libadblockplus-binaries/android_arm/libadblockplus.a

include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)

LOCAL_MODULE := v8-base
LOCAL_SRC_FILES :=  $(LOCAL_PATH)/libadblockplus-binaries/android_arm/libv8_base.a

include $(PREBUILT_STATIC_LIBRARY)




include $(CLEAR_VARS)

LOCAL_MODULE := v8-snapshot
LOCAL_SRC_FILES :=  $(LOCAL_PATH)/libadblockplus-binaries/android_arm/libv8_snapshot.a

include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)

LOCAL_MODULE := libadblocker
LOCAL_SRC_FILES := JniJsEngine.cpp JniFilterEngine.cpp JniJsValue.cpp
LOCAL_SRC_FILES += JniFilter.cpp JniSubscription.cpp
LOCAL_SRC_FILES += Utils.cpp

LOCAL_CPP_FEATURES := exceptions 
LOCAL_CPPFLAGS  += -std=gnu++0x


LOCAL_C_INCLUDES := $(LOCAL_PATH)/libadblockplus-binaries/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_LDLIBS := -ldl -llog
LOCAL_STATIC_LIBRARIES := adblockplus v8-base v8-snapshot

include $(BUILD_SHARED_LIBRARY)
