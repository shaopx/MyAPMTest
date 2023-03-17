//
// Created by shaopx on 2023/3/17.
//

#ifndef MYAPMTEST_ANRDUMPER_H
#define MYAPMTEST_ANRDUMPER_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <vector>
#include <mutex>

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "MyNativeLog"
#define ALOGD(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


class AnrDumper {
public:
    AnrDumper(const char *anrTraceFile, int i);
    ~AnrDumper();

    class Handler {
    public:
        virtual void handleSignal(int sig, siginfo_t* info, void* ucontext) = 0;
    };

    bool installHandlersLocked();
    static void sendSigToSignalCatcher();

//    void addSignalHandler(std::unique_ptr<AnrDumper::Handler> &&uniquePtr);
    void addSignalHandler(std::unique_ptr<AnrDumper::Handler> &&uniquePtr);

private:

    static void signalHandler(int sig, siginfo_t* info, void* ucontext);
    static int getSignalCatcherThreadId();
    std::string mAnrTraceFile;
    static int sSignalCatcherThreadId;
    sigset_t old_sigSet;
    static std::atomic<bool> sHandlerInstalled;
    static std::mutex sHandlerStackMutex;
    static std::vector<std::unique_ptr<Handler>> sHandlerStack;
};


#endif //MYAPMTEST_ANRDUMPER_H
