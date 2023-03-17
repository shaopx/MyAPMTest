package com.spx.myapmtest

import android.util.Log
import androidx.lifecycle.ViewModel
import kotlinx.coroutines.delay
import kotlinx.coroutines.runBlocking
import kotlin.concurrent.thread

class MainViewModel : ViewModel() {

    val TAG = "MainViewModel"
    fun simulateANR() {
        var look = Object();
        thread {
            synchronized(look) {
                Log.i(TAG, "child thread got the lock!")
                Thread.sleep(20000)
            }
        }
        Thread.sleep(1000);
        synchronized(look) {
            Log.i(TAG, "i got the lock!")
        }
    }
}