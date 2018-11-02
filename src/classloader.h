//
// Created by strunov on 8/24/2018.
//

#ifndef LIBRETRO_J_CLASSLOADER_H
#define LIBRETRO_J_CLASSLOADER_H

#include <jni.h>

int loadJar(JNIEnv *env, const char* pathToJar);
jclass loadClass(JNIEnv *env, const char* className);

#endif //LIBRETRO_J_CLASSLOADER_H
