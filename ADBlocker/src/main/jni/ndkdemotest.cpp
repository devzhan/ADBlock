//
// Created by xiuwen.zhan on 2019/6/18.
//
#include "com_android_zone_ndktools_NDKTools.h"
#include <android/log.h>
#define LOG_TAG "NDKTools"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


JNIEXPORT jstring JNICALL Java_com_android_zone_ndktools_NDKTools_callStaticMethod__I
        (JNIEnv * env, jclass jclass1, jint jint1){
    jclass  class_ndktools=env ->FindClass("com/android/zone/ndktools/NDKTools");
    if (class_ndktools==NULL){
        return NULL;
    }
    jmethodID  mth_static_method = env->GetStaticMethodID(class_ndktools,"staticMethod","(Ljava/lang/String;)Ljava/lang/String;");
    if (mth_static_method==NULL){
        return NULL;
    }
    jstring  data = env->NewStringUTF("native 调用java");
    env->CallStaticVoidMethod(class_ndktools,mth_static_method,data);
    env->DeleteLocalRef(class_ndktools);
    env->DeleteLocalRef(data);
    return data;

}


JNIEXPORT jstring JNICALL Java_com_android_zone_ndktools_NDKTools_getStringFromNDK
        (JNIEnv * env, jclass jclass1){
    jstring  data = env->NewStringUTF("hello this is jni");
    return data;
}

JNIEXPORT void JNICALL Java_com_android_zone_ndktools_NDKTools_callbackmethod(JNIEnv * env, jobject object) {

    //先获取字节码对象  jclass      (*FindClass)(JNIEnv*, const char*);
    jclass jclazz=env -> FindClass( "com/android/zone/ndktools/NDKTools");
    //获取method对象   jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID methodID = env->GetMethodID(jclazz, "helloFromJava", "()V");
    //通过字节码文件创建一个object对象（可选，方法中已经传递一个object），如果需要调用的方法与本地方法不在同一个文件夹则需要新创建object
    //jobject     (*AllocObject)(JNIEnv*, jclass);
    //通过对象调用方法 (调用java的是空方法，不用填写参数) void   (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    env->CallVoidMethod(object, methodID);
}

JNIEXPORT void JNICALL Java_com_android_zone_ndktools_NDKTools_callbackIntmethod
        (JNIEnv *env, jobject object){
    jclass clzz=env->FindClass("com/android/zone/ndktools/NDKTools");
    //获取method对象   jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID methodID=env->GetMethodID(clzz,"add","(II)I");

    int result=env->CallIntMethod(object,methodID,3,4);
    //logcat 打印相加返回的结果
    LOGD("RESLUT = %d",result);
}
JNIEXPORT void JNICALL Java_com_android_zone_ndktools_NDKTools_callbackStringmethod
        (JNIEnv *env, jobject object){
    jclass clzz=env->FindClass("com/android/zone/ndktools/NDKTools");
    //获取method对象   jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID methodID=env->GetMethodID(clzz,"printString","(Ljava/lang/String;)V");

    jstring result=env->NewStringUTF("hello form C/C++ ");
    env->CallVoidMethod(object,methodID,result);
}
JNIEXPORT void JNICALL Java_com_android_zone_ndktools_NDKTools_callStaticmethod
        (JNIEnv *env, jobject){
    //先获取字节码对象  jclass      (*FindClass)(JNIEnv*, const char*);
    jclass clzz=env->FindClass("com/android/zone/ndktools/NDKTools");
    jmethodID methodID=env->GetStaticMethodID(clzz,"staticmethod","(Ljava/lang/String;)V");
    jstring str = env->NewStringUTF( "C调用java");
    env->CallStaticVoidMethod(clzz,methodID,str);
}