//
// Created by strunov on 8/24/2018.
//

#include "classloader.h"
#include "retro-j.h"
#include "org_qw3rtrun_libretro_cb_LogCallbackNative.h"

jobject classLoader;
jmethodID load;

int loadJar(JNIEnv *env, const char* pathToJar){
    jstring jarPath = (*env)->NewStringUTF(env, pathToJar);

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

    load = (*env)->GetMethodID(env, loaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}

jclass loadClass(JNIEnv *env, const char* className) {
    jclass class = (*env)->CallObjectMethod(env, classLoader, load, (*env)->NewStringUTF(env, className));
    return class;
}