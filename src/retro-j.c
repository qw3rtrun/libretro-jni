#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "retro-j.h"
#include "classloader.h"
#include "org_qw3rtrun_libretro_cb_EnvironmentNative.h"
#include "org_qw3rtrun_libretro_cb_LogCallbackNative.h"
#include <jni.h>

JavaVM* jvm;
JNIEnv* env;
jclass Retro;
jobject retrojImpl;
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

    loadJar(env, info->path);
    initLoggerOnJarLoaded(env);


    jclass Retro = loadClass(env, "org.qw3rtrun.libretro.Retro");

    jmethodID retroGetInstance = (*env)->GetStaticMethodID(env, Retro, "getInstance", "(Lorg/qw3rtrun/libretro/cb/LogCallback;)Lorg/qw3rtrun/libretro/Retro;");
    if (!retroGetInstance) {
        retroj_log_cb(RETRO_LOG_ERROR, "Method is Not Found: org.qw3rtrun.libretro.Retro.getInstance");
        return false;
    }

    retrojImpl = (*env)->CallStaticObjectMethod(env, Retro, retroGetInstance, getLogger());

    return true;
}

void retro_deinit(void)
{
    (*jvm)->DestroyJavaVM(jvm);
}



