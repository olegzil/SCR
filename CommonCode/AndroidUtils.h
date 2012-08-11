#ifndef __COMMONUTILS__H__
#define __COMMONUTILS__H__
#ifdef OS_ANDROID
#include <android/log.h>
#include <iterator>
#define LOGV(TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__) 
#define LOGD(TAG, ...) __android_log_print(ANDROID_LOG_DEBUG  , TAG, __VA_ARGS__) 
#define LOGI(TAG, ...) __android_log_print(ANDROID_LOG_INFO   , TAG, __VA_ARGS__) 
#define LOGW(TAG, ...) __android_log_print(ANDROID_LOG_WARN   , TAG, __VA_ARGS__) 
#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR  , TAG, __VA_ARGS__) 
template<typename T>
void debugPrint(const char * tag,T& container)
{
    size_t index=0;
    for (typename T::iterator i=container.begin(); i!=container.end(); ++i)
    {
        LOGI(tag, "index = %d value = %f\n", index++, *i);
    }
    
}


#endif
#endif

