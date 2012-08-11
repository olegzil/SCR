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

#pick up all *.cpp files in the jni directory
#to specify a directory other then jni: FILE_LIST := $(wildcard $(LOCAL_PATH)/<your directory>/*.cpp)

#FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp) #command to pickup all *.cpp files in the current directory. does not recurse into sub-directories
#LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)  #second part of the previous command

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp) 				#picup all *.cpp files in the current directory
FILE_LIST += $(wildcard $(LOCAL_PATH)/**/*.cpp)				#pickup all *.cpp files a sub-dir one level down
FILE_LIST += $(wildcard $(LOCAL_PATH)/**/**/*.cpp)			#pickup all *.cpp files two levels down.
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)			#execute command

LOCAL_MODULE    := NativeCode
LOCAL_CFLAGS += -I/usr/local/include/libjson 
LOCAL_CFLAGS += -I../../CommonCode
LOCAL_CFLAGS += -I$(BOOST_PATH)
LOCAL_CFLAGS += -c -Os -ffast-math 
LOCAL_CFLAGS += -std=c++0x 
LOCAL_CFLAGS += -w
LOCAL_LDFLAGS += -Wl,-Map,xxx.map


LOCAL_CFLAGS +=   -I$(LOCAL_PATH) -I$(ANDROID_NDK)/sources/cxx-stl/gnu-libstdc++/include/ -D__ANDROID__   -D__STDC_INT64__
LOCAL_LDLIBS    := -llog  -lGLESv2 -lGLESv1_CM -lEGL -ldl


include $(BUILD_SHARED_LIBRARY)
