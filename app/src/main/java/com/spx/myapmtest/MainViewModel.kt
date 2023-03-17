package com.spx.myapmtest

import androidx.lifecycle.ViewModel
import kotlinx.coroutines.delay
import kotlinx.coroutines.runBlocking

class MainViewModel: ViewModel() {

    fun simulateANR() {
        runBlocking {
            // 模拟耗时操作，如网络请求或大量计算
            delay(20000) // 20 秒
        }
    }
}