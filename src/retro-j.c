#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "retro-j.h"
#include "org_qw3rtrun_libretro_cb_EnvironmentNative.h"
#include "org_qw3rtrun_libretro_cb_LogCallbackNative.h"
#include <jni.h>

JavaVM* jvm;
JNIEnv* env;
jobject classLoader;
jmethodID loadClass;
jclass Retro;
jobject retrojImpl;
jclass Logger;
jobject retrojLogger;
jclass Variable;
jmethodID newVariable;
jmethodID getName;
jmethodID getValue;

uint32_t* retroj_frame_buf;
retro_environment_t retroj_environment = 0;
retro_log_printf_t retroj_log_cb;

static bool use_audio_cb;
static float last_aspect;
static float last_sample_rate;

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
    (void)level;
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
}

static void loadJavaSymbolsFromJar() {
    Variable = (*env)->FindClass(env, "org/qw3rtrun/libretro/struct/Variable");
    newVariable = (*env)->GetMethodID(env, Variable, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    getName = (*env)->GetMethodID(env, Variable, "getName", "()Ljava/lang/String;Ljava/lang/String;");
    getValue = (*env)->GetMethodID(env, Variable, "getValue", "()Ljava/lang/String;Ljava/lang/String;");
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name     = "LibretroJ";
    info->library_version  = "v1";
    info->need_fullpath    = true;
    info->valid_extensions = "jar"; // any game should be packed as a single jar
}

void retro_init(void)
{
    JavaVMInitArgs args;
    JavaVMOption options[0];
    args.version = JNI_VERSION_9;
    args.nOptions = 0;
    args.options = options;
    args.ignoreUnrecognized = JNI_TRUE;

    jint result = JNI_CreateJavaVM(&jvm, (void **)&env, &args);
    if (result != JNI_OK) {
        char* error;
        fallback_log(RETRO_LOG_ERROR, error);
        exit(result);
    }
}

void retro_set_environment(retro_environment_t environment) {
    retroj_environment = environment;

    static struct retro_log_callback logging;

    if (environment(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging) && logging.log)
        retroj_log_cb = logging.log;
    else
        retroj_log_cb = fallback_log;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
    retroj_log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

bool retro_load_game(const struct retro_game_info *info) {

    if (!(info->path)) {
        retroj_log_cb(RETRO_LOG_ERROR, "Path of a Game Jar is Not Specified");
        return false;
    }
    jstring jarPath = (*env)->NewStringUTF(env, info->path);

    jclass fileClass = (*env)->FindClass(env, "java/io/File");
    if (!fileClass) {
        retroj_log_cb(RETRO_LOG_ERROR, "Class is Not Found: java.io.File");
        return false;
    }

    jmethodID fileNew = (*env)->GetMethodID(env, fileClass, "<init>", "(Ljava/lang/String;)V");
    if (!fileNew) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: java.io.File.<init>");
        return false;
    }

    jobject file = (*env)->NewObject(env, fileClass, fileNew, jarPath);
    if (!file) {
        retroj_log_cb(RETRO_LOG_ERROR, "Game File is Not Found");
        return false;
    }

    jmethodID fileToUri = (*env)->GetMethodID(env, fileClass, "toURI", "()Ljava/net/URI;");
    if (!fileToUri) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: java.io.File.toURI");
        return false;
    }

    jobject jarURI = (*env)->CallObjectMethod(env, file, fileToUri);

    jclass uriClass = (*env)->FindClass(env, "java/net/URI");
    if (!uriClass) {
        retroj_log_cb(RETRO_LOG_ERROR, "Class is Not Found: java.net.URI");
        return false;
    }

    jmethodID uriToUrl = (*env)->GetMethodID(env, uriClass, "toURL", "()Ljava/net/URL;");
    if (!uriToUrl) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: java.net.URI.toURL");
        return false;
    }

    jclass urlClass = (*env)->FindClass(env, "java/net/URL");
    if (!urlClass) {
        retroj_log_cb(RETRO_LOG_ERROR, "Class is Not Found: java.net.URL");
        return false;
    }

    jobject url = (*env)->CallObjectMethod(env, jarURI, uriToUrl);

    jclass loaderClass = (*env)->FindClass(env, "java/net/URLClassLoader");
    if (!loaderClass) {
        retroj_log_cb(RETRO_LOG_ERROR, "Class is Not Found: java.net.URLClassLoader");
        return false;
    }

    jarray urls = (*env)->NewObjectArray(env, 1, urlClass, url);

    jmethodID loaderNew = (*env)->GetStaticMethodID(env, loaderClass, "newInstance", "([Ljava/net/URL;)Ljava/net/URLClassLoader;");
    if (!loaderNew) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: java.net.URLClassLoader.newInstance");
        return false;
    }

    classLoader = (*env)->CallStaticObjectMethod(env, loaderClass, loaderNew, urls);

    loadClass = (*env)->GetMethodID(env, loaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jclass Retro = (*env)->CallObjectMethod(env, classLoader, loadClass, (*env)->NewStringUTF(env, "org.qw3rtrun.libretro.Retro"));

    jmethodID retroGetInstance = (*env)->GetStaticMethodID(env, Retro, "getInstance", "(Lorg/qw3rtrun/libretro/cb/LogCallback;)Lorg/qw3rtrun/libretro/Retro;");
    if (!retroGetInstance) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: org.qw3rtrun.libretro.Retro.getInstance");
        return false;
    }

    Logger = (*env)->CallObjectMethod(env, classLoader, loadClass, (*env)->NewStringUTF(env, "org.qw3rtrun.libretro.cb.LogCallbackNative"));
    if (!Logger) {
        retroj_log_cb(RETRO_LOG_ERROR, "Class is Not Found: org.qw3rtrun.libretro.cb.LogCallbackNative");
        return false;
    }

    static JNINativeMethod methods[] = {
            {"call",    "(ILjava/lang/String;)V",  (void *)&Java_org_qw3rtrun_libretro_cb_LogCallbackNative_call}
    };

    (*env)->RegisterNatives(env, Logger, methods, sizeof(methods)/sizeof(methods[0]));

    jmethodID loggerNew = (*env)->GetMethodID(env, Logger, "<init>", "()V");
    if (!loggerNew) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: org.qw3rtrun.libretro.Retro.getInstance");
        return false;
    }

    retrojLogger = (*env)->NewObject(env, Logger, loggerNew);

    retrojImpl = (*env)->CallStaticObjectMethod(env, Retro, retroGetInstance, retrojLogger);

    return true;
}

void retro_deinit(void)
{
    (*jvm)->DestroyJavaVM(jvm);
}



