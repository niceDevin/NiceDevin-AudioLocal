package com.nicedevin.audiolocal.ffmpeg

import android.util.Log
import com.arthenica.ffmpegkit.FFmpegKit
import com.arthenica.ffmpegkit.FFmpegKitConfig
import com.arthenica.ffmpegkit.FFmpegSessionCompleteCallback
import com.arthenica.ffmpegkit.FFprobeKit
import com.arthenica.ffmpegkit.ReturnCode
import com.arthenica.ffmpegkit.StatisticsCallback

/**
 * FFmpegKit（io.github.maitrungduc1410 维护分支）封装。
 * 库替换点隔离在这里：若未来改用 FFmpegKitNext，只改本文件。
 * 全程本地执行，不上传任何文件。
 */
object FFmpegController {

    @Volatile
    private var nativeReady: Boolean? = null

    @Volatile
    private var nativeErrorDetail: String? = null

    /**
     * 检测 FFmpeg 原生库能否加载（首次调用缓存结果）。
     * 任何原生层加载失败都会记录详细异常并返回 false，绝不抛出崩溃。
     */
    fun nativeAvailable(): Boolean {
        nativeReady?.let { return it }
        val ok = try {
            // 说明：fork 的 libffmpegkit.so 曾缺失 libc++_shared.so 依赖导致
            // libavcodec.so 加载失败（cannot locate symbol __gxx_personality_v0）。
            // 已在 app/src/main/jniLibs/ 用补丁过的库替换修复，此处无需额外预加载。
            FFmpegKitConfig.getVersion()
            true
        } catch (t: Throwable) {
            nativeErrorDetail = "${t.javaClass.simpleName}: ${t.message}"
            Log.e("AudioLocal", "FFmpeg native load failed", t)
            false
        }
        nativeReady = ok
        return ok
    }

    /** 最近一次原生加载失败的具体异常描述（用于诊断）。 */
    fun nativeErrorDetail(): String? = nativeErrorDetail

    /** 引擎版本号（用于调试展示）。 */
    fun engineVersion(): String = try {
        FFmpegKitConfig.getVersion()
    } catch (t: Throwable) {
        "unknown"
    }

    /** 探测音频时长（秒）；未知时返回 null。 */
    fun probeDurationSec(path: String): Double? = try {
        FFprobeKit.getMediaInformation(path)
            ?.mediaInformation
            ?.duration
            ?.toDoubleOrNull()
    } catch (t: Throwable) {
        null
    }

    /**
     * 异步转码：onProgress 0..99、onDone、onError。
     * 回调默认在主线程派发，可直接更新 Compose 状态。
     * 任何原生层异常都会走 onError，绝不抛出导致崩溃。
     */
    fun convert(
        input: String,
        output: String,
        args: List<String>,
        durationSec: Double?,
        onProgress: (Int) -> Unit,
        onDone: (String) -> Unit,
        onError: (String) -> Unit
    ) {
        val full = buildList {
            add("-y")
            add("-i")
            add(input)
            addAll(args)
            add(output)
        }
        try {
            FFmpegKit.executeWithArgumentsAsync(
                full.toTypedArray(),
                FFmpegSessionCompleteCallback { session ->
                    if (ReturnCode.isSuccess(session.returnCode)) {
                        onDone(output)
                    } else {
                        val detail = session.failStackTrace?.takeIf { it.isNotBlank() }
                            ?: "ffmpeg 退出码 ${session.returnCode?.value}"
                        onError(detail)
                    }
                },
                null,
                StatisticsCallback { stats ->
                    val d = durationSec
                    if (d != null && d > 0 && stats.time >= 0) {
                        onProgress(((stats.time / d) * 100).toInt().coerceIn(0, 99))
                    }
                }
            )
        } catch (t: Throwable) {
            onError(t.message ?: "转码引擎异常：${t.javaClass.simpleName}")
        }
    }
}
