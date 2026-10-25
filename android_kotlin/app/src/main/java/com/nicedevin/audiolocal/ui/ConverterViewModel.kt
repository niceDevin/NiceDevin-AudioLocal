package com.nicedevin.audiolocal.ui

import android.content.Context
import android.net.Uri
import android.provider.DocumentsContract
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.nicedevin.audiolocal.data.FileItem
import com.nicedevin.audiolocal.data.Status
import com.nicedevin.audiolocal.ffmpeg.FFmpegController
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.coroutines.withContext
import java.io.File
import java.util.concurrent.atomic.AtomicInteger
import kotlin.coroutines.resume

/** 从系统文件选择器取回的待处理文件。 */
data class PickedFile(val uri: Uri, val name: String, val size: Long)

class ConverterViewModel : ViewModel() {

    private val seq = AtomicInteger(0)

    val items = mutableStateListOf<FileItem>()

    var outputFormat by mutableStateOf("mp3")
    var bitrate by mutableStateOf("auto")
    var sampleRate by mutableStateOf("auto")
    var channels by mutableStateOf("auto")

    var converting by mutableStateOf(false)
    var coreError by mutableStateOf("")

    val readyCount: Int get() = items.count { it.status == Status.DONE }

    // ===== 文件列表 =====
    fun addFiles(list: List<PickedFile>) {
        list.filter { isAudioName(it.name) }.forEach {
            items.add(FileItem(seq.incrementAndGet(), it.uri, it.name, it.size))
        }
    }

    fun removeItem(id: Int) {
        val it = items.firstOrNull { i -> i.id == id } ?: return
        it.outFile?.delete()
        items.remove(it)
    }

    fun clearAll() {
        if (converting) return
        items.forEach { it.outFile?.delete() }
        items.clear()
        coreError = ""
    }

    private fun isAudioName(name: String): Boolean {
        val ext = name.substringAfterLast('.', "").lowercase()
        return ext in setOf("mp3", "flac", "wav", "m4a", "aac", "ogg", "opus", "amr", "wma", "ape", "wv")
    }

    // ===== 参数构建（与网页版 buildArgs 完全一致）=====
    fun buildArgs(): List<String> {
        val args = mutableListOf<String>()
        val br = bitrate
        if (br != "auto" && outputFormat in setOf("mp3", "m4a", "ogg", "opus")) {
            args += listOf("-b:a", "${br}k")
        }
        if (sampleRate != "auto") args += listOf("-ar", sampleRate)
        if (channels != "auto") args += listOf("-ac", if (channels == "mono") "1" else "2")

        val codec = when (outputFormat) {
            "mp3" -> listOf("-c:a", "libmp3lame")
            "flac" -> listOf("-c:a", "flac")
            "wav" -> listOf("-c:a", "pcm_s16le")
            "m4a" -> listOf("-c:a", "aac")
            "ogg" -> listOf("-c:a", "libvorbis")
            "opus" -> listOf("-c:a", "libopus")
            else -> emptyList()
        }
        args += codec
        return args
    }

    // ===== 转码 =====
    fun convertAll(context: Context) {
        if (converting || items.isEmpty()) return
        if (items.none { it.status == Status.IDLE }) return
        coreError = ""
        converting = true
        viewModelScope.launch {
            val nativeOk = FFmpegController.nativeAvailable()
            for (item in items) {
                if (item.status != Status.IDLE) continue
                item.status = Status.CONVERTING
                item.progress = 0
                item.err = ""
                if (!nativeOk) {
                    item.status = Status.ERROR
                    item.err = "本机 CPU 架构不支持本地 FFmpeg（需 arm64 / armeabi-v7a），请使用 ARM 手机或模拟器"
                    continue
                }
                val ok = try {
                    convertOne(context, item)
                } catch (t: Throwable) {
                    item.err = t.message?.takeIf { it.isNotBlank() }
                        ?: "转换发生异常：${t.javaClass.simpleName}"
                    false
                }
                if (ok) {
                    item.status = Status.DONE
                    item.progress = 100
                } else {
                    item.status = Status.ERROR
                }
            }
            converting = false
        }
    }

    private suspend fun convertOne(context: Context, item: FileItem): Boolean {
        val input = withContext(Dispatchers.IO) { copyInputToCache(context, item) }
            ?: return false.also { item.err = "读取文件失败" }

        val duration = withContext(Dispatchers.IO) {
            FFmpegController.probeDurationSec(input.absolutePath)
        }
        val output = withContext(Dispatchers.IO) {
            buildOutputFile(context, item.name)
        }
        val args = buildArgs()

        val err = awaitConvert(input.absolutePath, output.absolutePath, args, duration) { pct ->
            item.progress = pct
        }
        if (err.isEmpty()) {
            item.outFile = output
            return true
        }
        item.err = err
        return false
    }

    private fun copyInputToCache(context: Context, item: FileItem): File? = try {
        val ext = item.name.substringAfterLast('.', "").lowercase()
        val target = File(context.cacheDir, "input_${item.id}.$ext")
        context.contentResolver.openInputStream(item.uri)?.use { input ->
            target.outputStream().use { out -> input.copyTo(out) }
        }
        target
    } catch (e: Exception) {
        null
    }

    private fun buildOutputFile(context: Context, name: String): File {
        val stem = name.substringBeforeLast('.', name)
        val safe = stem.replace(Regex("[\\\\/:*?\"<>|]"), "_")
        return File(context.cacheDir, "$safe.$outputFormat")
    }

    private suspend fun awaitConvert(
        input: String,
        output: String,
        args: List<String>,
        duration: Double?,
        onProgress: (Int) -> Unit
    ): String = suspendCancellableCoroutine { cont ->
        FFmpegController.convert(
            input, output, args, duration,
            onProgress = onProgress,
            onDone = { cont.resume("") },
            onError = { e -> cont.resume(e) }
        )
        cont.invokeOnCancellation { }
    }

    // ===== 导出 / 下载（SAF，无需存储权限）=====
    fun exportOne(context: Context, item: FileItem, targetUri: Uri) {
        val out = item.outFile ?: return
        viewModelScope.launch(Dispatchers.IO) {
            try {
                context.contentResolver.openOutputStream(targetUri)?.use { os ->
                    out.inputStream().use { it.copyTo(os) }
                }
            } catch (e: Exception) {
                coreError = e.message ?: "导出失败"
            }
        }
    }

    fun exportAll(context: Context, treeUri: Uri) {
        val done = items.filter { it.status == Status.DONE && it.outFile != null }
        if (done.isEmpty()) return
        viewModelScope.launch(Dispatchers.IO) {
            val resolver = context.contentResolver
            for (it in done) {
                val out = it.outFile ?: continue
                try {
                    val parentDoc = DocumentsContract.buildDocumentUriUsingTree(
                        treeUri, DocumentsContract.getTreeDocumentId(treeUri)
                    )
                    val mime = mimeForExt(out.extension)
                    val doc = DocumentsContract.createDocument(resolver, parentDoc, mime, out.name) ?: continue
                    resolver.openOutputStream(doc)?.use { os ->
                        out.inputStream().use { ins -> ins.copyTo(os) }
                    }
                } catch (e: Exception) {
                    coreError = e.message ?: "导出失败"
                }
            }
        }
    }

    companion object {
        fun mimeForExt(ext: String): String = when (ext.lowercase()) {
            "mp3" -> "audio/mpeg"
            "flac" -> "audio/flac"
            "wav" -> "audio/wav"
            "m4a" -> "audio/mp4"
            "ogg", "opus" -> "audio/ogg"
            "aac" -> "audio/aac"
            else -> "application/octet-stream"
        }
    }
}
