package com.spx.myapmtest

import android.app.Application
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.activity.viewModels
import androidx.lifecycle.lifecycleScope
import com.spx.myapmtest.databinding.ActivityMainBinding
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {
    private val TAG = "MainActivity";
    private lateinit var binding: ActivityMainBinding
    private val viewModel: MainViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        sApplication = application;
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        var helper = APMHelper()

        // 当前只演示了发生anr时的错误信息收集
        var useMyAnrTrace = true
        if (useMyAnrTrace) {
            var signalCatcherTid = helper.getSignalCatcherThreadId()
            Log.d(TAG, "SignalCatcherThreadId:$signalCatcherTid")
            NativeInterface.initAnrDumper("my_anr_trace.txt", signalCatcherTid)

            val mySignalHandler = MyAnrSignalHandler()
            NativeInterface.addSignalHandler(mySignalHandler)
        }

        // anr演示
        binding.buttonSimulateAnr.setOnClickListener {
            lifecycleScope.launch {
                viewModel.simulateANR()
            }
        }

        // crash
        binding.buttonSimulateCrash.setOnClickListener {
            // ToDo
        }

        // 卡顿
        binding.buttonSimulateLag.setOnClickListener {
            //todo
        }
    }

    companion object {
        lateinit var sApplication: Application;
    }
}