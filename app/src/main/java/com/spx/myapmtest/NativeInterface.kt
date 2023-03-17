package com.spx.myapmtest

object NativeInterface {

    external fun stringFromJNI(): String

    external fun simulateANR(): String

    external fun initAnrDumper(anrTraceFile: String, signalCatcherThreadId: Int)

    external fun addSignalHandler(anrSignalHandler: MyAnrSignalHandler)

    init {
        System.loadLibrary("myapmtest")
    }
}