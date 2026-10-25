package com.nicedevin.audiolocal.data

import android.net.Uri
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import java.io.File

enum class Status { IDLE, CONVERTING, DONE, ERROR }

/** 单个待转换文件（状态用 Compose 可观察状态承载）。 */
class FileItem(
    val id: Int,
    val uri: Uri,
    val name: String,
    val size: Long
) {
    var progress by mutableStateOf(0)
    var status by mutableStateOf(Status.IDLE)
    var outFile: File? by mutableStateOf(null)
    var err by mutableStateOf("")
}
