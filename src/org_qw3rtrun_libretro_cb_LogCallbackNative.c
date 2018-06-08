#include "org_qw3rtrun_libretro_cb_LogCallbackNative.h"
#include "retro-j.h"

/*
 * Class:     org_qw3rtrun_libretro_cb_LogCallbackNative
 * Method:    call
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_qw3rtrun_libretro_cb_LogCallbackNative_call
        (JNIEnv *env, jobject logger, jint level, jstring msg) {
    const char *chars = (*env)->GetStringUTFChars(env, msg, JNI_FALSE);
    retroj_log_cb(level, chars);
}


