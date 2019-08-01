#include "org_qw3rtrun_libretro_cb_EnvironmentNative.h"
#include "org_qw3rtrun_libretro_cb_LogCallbackNative.h"
#include "retro-j.h"

JNIEXPORT jobject JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_getLogInterface
        (JNIEnv *env, jobject envNative) {
    return getLogger();
}

JNIEXPORT jobject JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_getVariable
        (JNIEnv *env, jobject this, jstring varName) {
    const char *varNameChars = (*env)->GetStringUTFChars(env, varName, JNI_FALSE);
    struct retro_variable var = {.key = varNameChars};
    if (retroj_environment(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value) {
        jstring value = (*env)->NewStringUTF(env, var.value);
        return (*env)->NewObject(env, Variable, newVariable, varName, value);
    }

    return NULL;
}

JNIEXPORT void JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_setVariables
        (JNIEnv *env, jobject this, jobject vars) {
    jclass Collection = (*env)->FindClass(env, "java/util/Collection");
    if (!Collection) {
        return;
    }

    jmethodID toArray = (*env)->GetMethodID(env, Collection, "toArray", "()[Ljava/lang/Object;");
    if (!toArray) {
        return;
    }

    jobjectArray array = (*env)->CallObjectMethod(env, vars, toArray);

    int size = (*env)->GetArrayLength(env, array);

    struct retro_variable rvars[size];

    for (int i = 0; i < size; ++i) {
        jobject v = (*env)->GetObjectArrayElement(env, array, i);
        jstring name = (*env)->CallObjectMethod(env, v, getName);
        jstring value = (*env)->CallObjectMethod(env, v, getValue);
        rvars[i].key = (*env)->GetStringUTFChars(env, name, NULL);
        rvars[i].value = (*env)->GetStringUTFChars(env, value, NULL);
    }

    retroj_environment(RETRO_ENVIRONMENT_SET_VARIABLES, &rvars);
}

JNIEXPORT jboolean JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_setPixelFormat0
        (JNIEnv * env, jobject this, jint pixelFormat) {
    enum retro_pixel_format rpf = (enum retro_pixel_format) pixelFormat;
    retroj_environment(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rpf);
}

JNIEXPORT jobject JNICALL Java_org_qw3rtrun_libretro_cb_EnvironmentNative_getCurrentSoftwareFrameBuffer0
        (JNIEnv * env, jobject this) {
    // TODO retroj_environment(RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER )
    return NULL;
}

