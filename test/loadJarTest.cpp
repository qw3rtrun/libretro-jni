//
// Created by qw3rtrun on 23.05.18.
//

#define BOOST_TEST_MODULE libretro-j-test

#include <boost/test/included/unit_test.hpp>
#include <stdio.h>
#include "src/retro-j.h"
#include <jni.h>
#include "src/org_qw3rtrun_libretro_cb_EnvironmentNative.h"

JavaVM *jvm;                      // Pointer to the JVM (Java Virtual Machine)
JNIEnv *env;                      // Pointer to native interface

void startJvm() {
    //================== prepare loading of Java VM ============================
    JavaVMInitArgs vm_args;                        // Initialization arguments
    JavaVMOption* options = new JavaVMOption[2];   // JVM invocation options
    options[0].optionString = const_cast<char *>("-Djava.class.path=../libretro.jar");   // where to find java .class
    options[1].optionString = const_cast<char *>("-verbose:jni;");   // where to find java .class
    vm_args.version = JNI_VERSION_9;             // minimum Java version
    vm_args.nOptions = 1;                          // number of options
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
    //=============== load and initialize Java VM and JNI interface =============
    BOOST_CHECK_EQUAL(JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args), JNI_OK); // YES !!
    delete options; // we then no longer need the initialisation options.
}

void stopJvm() {
    BOOST_CHECK_EQUAL(jvm->DestroyJavaVM(), JNI_OK);
}

boolean javaEquals(JNIEnv *env, jobject a, jobject b) {
    jclass objClass = env->FindClass("java/lang/Object");
    BOOST_CHECK_NE(objClass, nullptr);
    jmethodID equalsMethod = env->GetMethodID(objClass, "equals", "(Ljava/lang/Object;)Z");
    BOOST_CHECK_NE(equalsMethod, nullptr);
    jboolean eq = env->CallBooleanMethod(a, equalsMethod, b);
    return eq;
}


BOOST_AUTO_TEST_CASE(CheckRetroStub) {
    startJvm();
    jclass stubClass = env->FindClass("RetroStub");
    BOOST_CHECK_NE(stubClass, nullptr);
    jmethodID stubConstructor = env->GetMethodID(stubClass, "<init>", "()V");
    BOOST_CHECK_NE(stubConstructor, nullptr);
    jobject stub = env->NewObject(stubClass, stubConstructor);
    BOOST_CHECK_NE(stub, nullptr);
    jmethodID apiVersionMethod = env->GetMethodID(stubClass, "apiVersion", "()I");
    BOOST_CHECK_NE(apiVersionMethod, nullptr);
    jint apiVersion = env->CallIntMethod(stub, apiVersionMethod);
    BOOST_CHECK_EQUAL(apiVersion, RETRO_API_VERSION);

    jclass envClass = env->FindClass("org/qw3rtrun/libretro/cb/EnvironmentNative");
    BOOST_CHECK_NE(envClass, nullptr);

    static JNINativeMethod methods[] = {
            {"getVariable",    "(Ljava/lang/String;)Lorg/qw3rtrun/libretro/struct/Variable;",  (void *)&Java_org_qw3rtrun_libretro_cb_EnvironmentNative_getVariable}
    };

    env->RegisterNatives(envClass, methods, sizeof(methods)/sizeof(methods[0]));

    jmethodID envConstructor = env->GetMethodID(envClass, "<init>", "()V");
    BOOST_CHECK_NE(envConstructor, nullptr);
    jobject nativeEnv = env->NewObject(envClass, envConstructor);
    BOOST_CHECK_NE(nativeEnv, nullptr);
    jmethodID getVarMethod = env->GetMethodID(envClass, "getVariable", "(Ljava/lang/String;)Lorg/qw3rtrun/libretro/struct/Variable;");
    BOOST_CHECK_NE(getVarMethod, nullptr);

    jstring varName = env->NewStringUTF("test");

    jobject var = env->CallObjectMethod(nativeEnv, getVarMethod, varName);
    BOOST_CHECK_NE(var, nullptr);
    jclass varClass = env->FindClass("org/qw3rtrun/libretro/struct/Variable");
    BOOST_CHECK_NE(varClass, nullptr);
    jmethodID getValueMethod = env->GetMethodID(varClass, "getValue", "()Ljava/lang/String;");
    BOOST_CHECK_NE(getValueMethod, nullptr);
    jobject value = env->CallObjectMethod(var, getValueMethod);
    BOOST_CHECK(javaEquals(env, value, varName));

    stopJvm();
}

//int main() {
// startJvm();
//    return 0;
//}
