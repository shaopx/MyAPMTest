
#include "JniSignalHandler.h"


void JniSignalHandler::callJavaMethod(int sig) {
    ALOGD("jni JniSignalHandler::callJavaMethod...%d", sig);
    JNIEnv* env;
    jint attachResult = mJVM->AttachCurrentThread(&env, nullptr);
//    ALOGD("jni JniSignalHandler::callJavaMethod...attachResult:%d", attachResult);
    if (attachResult != JNI_OK) {
        // 处理附加失败的情况，例如记录错误
        return;
    }

    jclass handlerClass = env->GetObjectClass(mHandler);
//    ALOGD("jni JniSignalHandler::callJavaMethod...GetObjectClass");
    jmethodID handleSignalMethod = env->GetMethodID(handlerClass, "handleSignal", "(I)V");
//    ALOGD("jni JniSignalHandler::callJavaMethod...GetMethodID");
    if (handleSignalMethod == nullptr) {
        // 处理错误
        return;
    }

    env->CallVoidMethod(mHandler, handleSignalMethod, sig);

    mJVM->DetachCurrentThread();
}

void JniSignalHandler::handleSignal(int sig, siginfo_t *info, void *ucontext_t) {
    ALOGD("jni JniSignalHandler::handleSignal...");
    std::thread javaCallbackThread(&JniSignalHandler::callJavaMethod, this, sig);
    javaCallbackThread.detach();
}
