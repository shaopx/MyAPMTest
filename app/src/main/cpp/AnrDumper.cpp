

#include "AnrDumper.h"

std::atomic<bool> AnrDumper::sHandlerInstalled{false};
std::mutex AnrDumper::sHandlerStackMutex;
std::vector<std::unique_ptr<AnrDumper::Handler>> AnrDumper::sHandlerStack;

int AnrDumper::sSignalCatcherThreadId = 0;

AnrDumper::AnrDumper(const char *anrTraceFile, int signal_catcher_thread_id) {
    mAnrTraceFile = anrTraceFile;
    AnrDumper::sSignalCatcherThreadId = signal_catcher_thread_id;

    sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet, SIGQUIT);
    pthread_sigmask(SIG_UNBLOCK, &sigSet, &old_sigSet);

    installHandlersLocked();
}

bool AnrDumper::installHandlersLocked() {
    // 构造一个sigaction，将方法地址设置成我们的signalHandler
    struct sigaction sa{};
    sa.sa_sigaction = signalHandler;
    sa.sa_flags = SA_ONSTACK | SA_SIGINFO | SA_RESTART;
    // 设置一个新的sigaction
    if (sigaction(SIGQUIT, &sa, nullptr) == -1) {
        return false;
    }
    sHandlerInstalled = true;
    return true;
}

void AnrDumper::signalHandler(int sig, siginfo_t *info, void *uc) {
    std::unique_lock<std::mutex> lock(sHandlerStackMutex);
    ALOGD("my singal handler func.....sHandlerStack.size:%d", sHandlerStack.size());

    // 遍历注册的处理函数进行自定义处理,
    // 当前代码逻辑是从java层注册了一个处理器, 所以此处会回调到java层进行处理
    for (auto &handler: sHandlerStack) {
        handler->handleSignal(sig, info, uc);
    }
    lock.unlock();

    // 可以选择重新把信号发给SignalCatcher线程
    // sendSigToSignalCatcher();
}

int AnrDumper::getSignalCatcherThreadId() {
    // 这个值要想办法在native层获取, 不能在java层通过Thread.id获取, 这是两回事.
    return AnrDumper::sSignalCatcherThreadId;
}

void AnrDumper::sendSigToSignalCatcher() {
    int tid = getSignalCatcherThreadId();
    syscall(SYS_tgkill, getpid(), tid, SIGQUIT);
}

AnrDumper::~AnrDumper() {
    // You can restore the signal mask if needed
    pthread_sigmask(SIG_SETMASK, &old_sigSet, nullptr);
}

void AnrDumper::addSignalHandler(std::unique_ptr<AnrDumper::Handler> &&handler) {
    sHandlerStack.push_back(std::move(handler));
}
