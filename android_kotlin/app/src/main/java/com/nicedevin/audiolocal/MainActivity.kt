package com.nicedevin.audiolocal

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import com.nicedevin.audiolocal.ffmpeg.FFmpegController
import com.nicedevin.audiolocal.ui.App
import com.nicedevin.audiolocal.ui.theme.AudioLocalTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        // 启动即检测 FFmpeg 原生库，异常详情打到 logcat（tag=AudioLocal）便于诊断
        val ok = FFmpegController.nativeAvailable()
        Log.i("AudioLocal", "FFmpeg native available: $ok" +
            (FFmpegController.nativeErrorDetail()?.let { " | $it" } ?: ""))
        setContent {
            AudioLocalTheme {
                App()
            }
        }
    }
}
