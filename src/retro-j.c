#include "retro-j.h"
#include "org_qw3rtrun_libretro_EnvironmentNative.h"
#include <jni.h>

int f(int i) {
    return i+1;
}

retro_environment_t retro_environment = 0;

void retro_set_environment(retro_environment_t environment) {
    retro_environment = retro_environment;
}

JNIEXPORT jobject JNICALL Java_org_qw3rtrun_libretro_EnvironmentNative_getVariable
        (JNIEnv *env, jobject o, jstring varName) {
    jclass varType = (*env)->FindClass(env, "org/qw3rtrun/libretro/Variable");
    if (varType) {
        jmethodID varInit  = (*env)->GetMethodID(env, varType, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
        if (varInit) {
            return (*env) -> NewObject(env, varType, varInit, varName, varName);
        }
    }
    return NULL;
}

