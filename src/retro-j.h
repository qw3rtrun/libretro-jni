#ifndef RETROJ_H
#define RETROJ_H

#include "libretro.h"
#include <jni.h>

#ifdef __cplusplus
extern "C"{
#endif

extern jclass Retro;
extern jobject retrojImpl;
extern jclass Logger;
extern jobject retrojLogger;
extern jclass Variable;
extern jmethodID newVariable;
extern jmethodID getName;
extern jmethodID getValue;

extern uint32_t* retroj_frame_buf;
extern retro_environment_t retroj_environment;
extern retro_log_printf_t retroj_log_cb;



#ifdef __cplusplus
}
#endif

#endif