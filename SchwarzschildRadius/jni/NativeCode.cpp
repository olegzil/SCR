/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "TouchInputManager.h"
#include "Renderer.h"

#define  LOG_TAG    "libNativeCode =-=-=-="
static InputManager* gInputManager=NULL;
static Renderer* gRenderer = NULL;

extern "C" {                    
    JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_SceneGraphData(JNIEnv * env, jobject obj,  jbyteArray buffer, jint length);
    JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_touchInput(JNIEnv * env, jobject obj, jint evt_action, jfloat x, jfloat y, jint index, jlong time);
};

JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    gRenderer = Renderer::Instance();
    int side = std::max(width, height);
    Renderer::Instance()->setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_destroy(JNIEnv * env, jobject obj)
{
    Renderer::DestroyInstance();
    InputManager::DestroyInstance();
}

JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_step(JNIEnv * env, jobject obj)
{
    Renderer::Instance()->renderFrame();
}
JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_touchInput(JNIEnv * env, jobject obj, jint evt_action, jfloat x, jfloat y, jint index, jlong time)
{
    Notification::TouchEvent event;
    event.x = x;
    event.y = y;
    event.action = (Notification::ActionType)evt_action;
    event.time = time;
    event.index = index;
    
    char* action;
    switch(event.action)
    {
        case 0:
            event.action = Notification::ACTION_DOWN;
            action = "DOWN";
            break;
        case 1:
            event.action = Notification::ACTION_UP;
            action = "UP";
            break;
        case 2:
            event.action = Notification::ACTION_MOVE;
            action = "MOVE";
            break;
        case 5:
            event.action = Notification::ACTION_POINTER_UP;
            action = "POINTER_UP";
            break;
        case 6:
            event.action = Notification::ACTION_POINTER_DOWN;
            action = "POINTER_DOWN";
            break;
        default:
            event.action = Notification::ACTION_UNKNOWN;
            action = "UNKNOWN";
            break;
    }
    InputManager::Instance()->EventLoop(event);
}
JNIEXPORT void JNICALL Java_com_game_schwarzschildradius_NativeEntry_SceneGraphData(JNIEnv * env, jobject obj,  jbyteArray data, jint length)
{
     if (length<=0)
        return;
     jbyte *str = env->GetByteArrayElements(data, NULL);
     if (str == NULL) {
         return; /* OutOfMemoryError already thrown */
     }
     gRenderer = Renderer::Instance();
     gRenderer->UpdateSceneGraph((const char*)str, length);
     env->ReleaseByteArrayElements(data, str, 0);

}
