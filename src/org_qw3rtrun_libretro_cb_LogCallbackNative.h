/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_qw3rtrun_libretro_cb_LogCallbackNative */

#ifndef _Included_org_qw3rtrun_libretro_cb_LogCallbackNative
#define _Included_org_qw3rtrun_libretro_cb_LogCallbackNative
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_qw3rtrun_libretro_cb_LogCallbackNative
 * Method:    call
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_qw3rtrun_libretro_cb_LogCallbackNative_call
  (JNIEnv *, jobject, jint, jstring);

void initLoggerOnJarLoaded(JNIEnv *env);

jobject getLogger();

#ifdef __cplusplus
}
#endif
#endif
