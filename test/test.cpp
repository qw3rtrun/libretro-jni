//
// Created by qw3rtrun on 23.05.18.
//

//#define BOOST_TEST_MODULE libretro-j-test
//
//#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <stdio.h>
#include "../src/library.h"
#include <jni.h>
//
//
void startJvm() {
    JavaVM *jvm;                      // Pointer to the JVM (Java Virtual Machine)
    JNIEnv *env;                      // Pointer to native interface
    //================== prepare loading of Java VM ============================
    JavaVMInitArgs vm_args;                        // Initialization arguments
    JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
    options[0].optionString = const_cast<char *>("-Djava.class.path=libretro.jar");   // where to find java .class
    vm_args.version = JNI_VERSION_1_6;             // minimum Java version
    vm_args.nOptions = 1;                          // number of options
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
    //=============== load and initialize Java VM and JNI interface =============
    printf("TRYING..");
    jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
    delete options;    // we then no longer need the initialisation options.
    if (rc != JNI_OK) {
        // TO DO: error processing...
        printf("ERROR");
        exit(EXIT_FAILURE);
    }
    //=============== Display JVM version =======================================
    std::cout << "JVM load succeeded: Version ";
    jint ver = env->GetVersion();

    std::cout << ((ver>>16)&0x0f) << "."<<(ver&0x0f) << std::endl;

    jvm->DestroyJavaVM();
}

//BOOST_AUTO_TEST_CASE(PassTest) {
//    startJvm();
//}
int main() {
 startJvm();
    return 0;
}
