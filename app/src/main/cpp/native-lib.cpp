#include <jni.h>
#include <string>
#include "AnrDumper.h"
#include "JniSignalHandler.h"

static AnrDumper *anrDumper = nullptr;
// 在全局范围内定义一个JavaVM*变量
JavaVM* g_jvm = nullptr;


// 实现JNI_OnLoad函数
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* jvm, void* reserved) {
    // 将传入的JavaVM*保存到全局变量中
    g_jvm = jvm;

    // 返回JNI版本，通常是JNI_VERSION_1_6
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_spx_myapmtest_NativeInterface_simulateANR(JNIEnv *env, jobject thiz) {
    // TODO: implement simulateANR()
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_spx_myapmtest_NativeInterface_initAnrDumper(JNIEnv *env, jobject thiz,
                                                     jstring anrTraceFile,
                                                     jint signal_catcher_thread_id) {
    const char *traceFile = env->GetStringUTFChars(anrTraceFile, nullptr);
    anrDumper = new AnrDumper(traceFile, signal_catcher_thread_id);
    env->ReleaseStringUTFChars(anrTraceFile, traceFile);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_spx_myapmtest_NativeInterface_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_spx_myapmtest_NativeInterface_addSignalHandler(JNIEnv *env, jobject thiz,
                                                        jobject handler) {
    std::unique_ptr<JniSignalHandler> jniHandler(new JniSignalHandler(g_jvm, env, handler));
    anrDumper->addSignalHandler(std::move(jniHandler));
}