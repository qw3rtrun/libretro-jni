#include "org_qw3rtrun_libretro_cb_EnvironmentNative.h"
#include "retro-j.h"

JNIEXPORT jobject JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_getLogInterface
        (JNIEnv *env, jobject envNative) {
}

JNIEXPORT jobject JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_getVariable
        (JNIEnv *env, jobject o, jstring varName) {
    jclass varType = (*env)->FindClass(env, "org/qw3rtrun/libretro/Variable");
    if (varType) {
        const char *varNameChars = (*env) -> GetStringUTFChars(env, varName, JNI_FALSE);
        struct retro_variable var = { .key = varNameChars};
        if (retroj_environment(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
            jmethodID varInit = (*env)->GetMethodID(env, varType, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
            if (varInit) {
                jstring value = (*env)->NewStringUTF(env, var.value);
                return (*env)->NewObject(env, varType, varInit, varName, value);
            }
        }
    }
    return NULL;
}
