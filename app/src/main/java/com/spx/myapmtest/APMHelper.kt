package com.spx.myapmtest

import android.util.Log

const val CHECK_ERROR_STATE_COUNT = 4
const val CHECK_ERROR_STATE_INTERVAL = 500L

class APMHelper {
    val TAG = "APMHelper"

    // 得到Signal Catcher线程id,
    fun getSignalCatcherThreadId(): Int {
        var signalCatcherTid = -1
        val threads: Set<Thread> = Thread.getAllStackTraces().keys
        for (thread in threads) {
            Log.d(TAG, "thread id:" + thread.id.toInt())
            if (thread.name == "Signal Catcher") {
                signalCatcherTid = thread.id.toInt()
                break
            }
        }
        return signalCatcherTid
    }

}