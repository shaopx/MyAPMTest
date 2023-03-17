//
// Created by shaopx on 2023/3/17.
//

#ifndef MYAPMTEST_JNISIGNALHANDLER_H
#define MYAPMTEST_JNISIGNALHANDLER_H

#include <jni.h>
#include <signal.h>
#include <thread>
#include "AnrDumper.h"

class JniSignalHandler: public AnrDumper::Handler {
public:
    JniSignalHandler(JavaVM* jvm, JNIEnv *env, jobject handler)
            : mEnv(env), mJVM(jvm), mHandler(env->NewGlobalRef(handler)) {
    }

    ~JniSignalHandler() {
        mEnv->DeleteGlobalRef(mHandler);
    }

    void handleSignal(int sig, siginfo_t *info, void *ucontext_t) ;
    void callJavaMethod(int sig) ;

private:
    JNIEnv *mEnv;
    JavaVM *mJVM;
    jobject mHandler;
};

#endif //MYAPMTEST_JNISIGNALHANDLER_H
