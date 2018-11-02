#include "org_qw3rtrun_libretro_cb_LogCallbackNative.h"
#include "retro-j.h"
#include "classloader.h"

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

jclass Logger;
jobject logger;

void initLoggerOnJarLoaded(JNIEnv *env) {
    Logger = loadClass(env, "org.qw3rtrun.libretro.cb.LogCallbackNative");
    if (!Logger) {
        retroj_log_cb(RETRO_LOG_ERROR, "Class is Not Found: org.qw3rtrun.libretro.cb.LogCallbackNative");
    }

    static JNINativeMethod methods[] = {
            {"call",    "(ILjava/lang/String;)V",  (void *)&Java_org_qw3rtrun_libretro_cb_LogCallbackNative_call}
    };

    (*env)->RegisterNatives(env, Logger, methods, sizeof(methods)/sizeof(methods[0]));

    jmethodID loggerNew = (*env)->GetMethodID(env, Logger, "<init>", "()V");
    if (!loggerNew) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: org.qw3rtrun.libretro.Retro.getInstance");
    }

    logger = (*env)->NewObject(env, Logger, loggerNew);
}

jobject getLogger() {
    return logger;
}


