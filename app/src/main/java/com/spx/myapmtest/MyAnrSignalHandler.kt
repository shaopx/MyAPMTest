package com.spx.myapmtest

import android.app.ActivityManager
import android.app.Application
import android.content.Context
import android.os.Process
import android.util.Log

/**
 * anr发生后,自定义的处理逻辑
 */
class MyAnrSignalHandler {

    val TAG = "MyAnrSignalHandler"

    /**
     * 可以在这里对应用是否真的是anr了, 进行进一步判断
     * 然后可以组织anr数据上报服务器, 或者保存到本地文件
     */
    fun handleSignal(sig: Int) {
        Log.i(TAG, "java/kt handleSignal: ....")
        report()
    }

    private fun collectThreadStackTraces(): String {
        val allStackTraces = Thread.getAllStackTraces()
        val stringBuilder = StringBuilder()

        for ((thread, stackTraceElements) in allStackTraces) {
            stringBuilder.append("Thread: ${thread.toString()}\n")
            stringBuilder.append("      : ${thread.name} ${Process.myTid()} ${thread.id}\n")
            for (stackTraceElement in stackTraceElements) {
                stringBuilder.append("\tat $stackTraceElement\n")
            }
            stringBuilder.append("\n")
        }

        return stringBuilder.toString()
    }

    private fun checkErrorStateCycle() {
        var checkErrorStateCount = 0
        while (checkErrorStateCount < CHECK_ERROR_STATE_COUNT) {
            try {
                checkErrorStateCount++
                val myAnr = checkErrorState(MainActivity.sApplication)
                //  如果有NOT_RESPONDING标记，马上上报
                if (myAnr) {
                    report()
                    break
                }
                Thread.sleep(CHECK_ERROR_STATE_INTERVAL)
            } catch (t: Throwable) {
                Log.e(TAG, "checkErrorStateCycle error, e : " + t.message)
                break
            }
        }
    }

    private fun report() {
        Log.i(TAG, "report anr stack trace:" + collectThreadStackTraces())
    }

    private fun checkErrorState(application: Application): Boolean {
//        Log.i(TAG, "checkErrorState: ....application:" + application)
        try {
            val am = application
                .getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
//            Log.i(TAG, "checkErrorState: ....am:" + am)
            val procs = am.processesInErrorState
            Log.i(TAG, "checkErrorState: ....procs:" + procs)
            if (procs == null) {
                return false;
            }
            for (proc in procs) {
                if (proc.uid != Process.myUid()
                    && proc.condition == ActivityManager.ProcessErrorStateInfo.NOT_RESPONDING
                ) {
                    Log.i(TAG, "maybe received other apps ANR signal")
                    return false
                }
                // 当前进程，且状态是NOT_RESPONDING，才返回true
                if (proc.pid != Process.myPid()) continue
                if (proc.condition != ActivityManager.ProcessErrorStateInfo.NOT_RESPONDING) {
                    continue
                }
                return true
            }
            return false
        } catch (t: Throwable) {
            Log.e(TAG, "[checkErrorState] error :" + t.message)
        }
        return false
    }
}