package com.nicedevin.audiolocal.ui

import android.net.Uri
import android.provider.OpenableColumns
import androidx.activity.compose.ManagedActivityResultLauncher
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExposedDropdownMenuAnchorType
import androidx.compose.material3.ExposedDropdownMenuBox
import androidx.compose.material3.ExposedDropdownMenuDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.OutlinedTextFieldDefaults
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.key
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontWeight
import com.nicedevin.audiolocal.R
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.text.style.TextDecoration
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.nicedevin.audiolocal.data.FileItem
import com.nicedevin.audiolocal.data.Status
import com.nicedevin.audiolocal.ui.theme.Accent
import com.nicedevin.audiolocal.ui.theme.AccentDeep
import com.nicedevin.audiolocal.ui.theme.AccentSoft
import com.nicedevin.audiolocal.ui.theme.Err
import com.nicedevin.audiolocal.ui.theme.ErrSoft
import com.nicedevin.audiolocal.ui.theme.Ink
import com.nicedevin.audiolocal.ui.theme.InkFaint
import com.nicedevin.audiolocal.ui.theme.InkSoft
import com.nicedevin.audiolocal.ui.theme.Line
import com.nicedevin.audiolocal.ui.theme.LineStrong
import com.nicedevin.audiolocal.ui.theme.Ok
import com.nicedevin.audiolocal.ui.theme.OkSoft
import com.nicedevin.audiolocal.ui.theme.Paper
import com.nicedevin.audiolocal.ui.theme.Surface

private val formatExts = listOf("mp3", "flac", "wav", "m4a", "ogg", "opus")

@Composable
fun ConverterPanel(vm: ConverterViewModel) {
    val context = LocalContext.current
    val strings = LocalStrings.current

    var exportTarget by remember { mutableStateOf<FileItem?>(null) }

    val pickLauncher = rememberLauncherForActivityResult(
        ActivityResultContracts.OpenMultipleDocuments()
    ) { uris ->
        if (uris.isNotEmpty()) {
            val picked = uris.map { uri -> PickedFile(uri, queryName(context, uri), querySize(context, uri)) }
            vm.addFiles(picked)
        }
    }

    val exportAllLauncher = rememberLauncherForActivityResult(
        ActivityResultContracts.OpenDocumentTree()
    ) { uri -> uri?.let { vm.exportAll(context, it) } }

    // 随输出格式重建，保证 SAF 保存对话框的 mime/文件名对应当前格式
    var exportOneLauncher by remember { mutableStateOf<ManagedActivityResultLauncher<String, Uri?>?>(null) }
    key(vm.outputFormat) {
        exportOneLauncher = rememberLauncherForActivityResult(
            ActivityResultContracts.CreateDocument(ConverterViewModel.mimeForExt(vm.outputFormat))
        ) { uri -> uri?.let { t -> exportTarget?.let { vm.exportOne(context, it, t) } } }
    }

    Column(
        modifier = Modifier
            .fillMaxWidth()
            .background(Surface, RoundedCornerShape(12.dp))
            .border(1.dp, Line, RoundedCornerShape(12.dp))
            .padding(20.dp)
    ) {
        // 面板头
        Row(verticalAlignment = Alignment.CenterVertically) {
            Text(strings.panelTitle, color = Ink, style = MaterialTheme.typography.headlineSmall, fontWeight = FontWeight.Bold)
            Spacer(Modifier.weight(1f))
            Text(strings.multiNote(vm.items.size), color = InkFaint, style = MaterialTheme.typography.bodySmall)
        }
        Spacer(Modifier.height(16.dp))

        // 选择区（点击打开文件选择器）
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .border(1.5.dp, if (vm.converting) Line else LineStrong, RoundedCornerShape(10.dp))
                .clickable(enabled = !vm.converting) { pickLauncher.launch(arrayOf("audio/*")) }
                .padding(vertical = 30.dp, horizontal = 16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Image(
                painter = painterResource(R.drawable.audiolocal_icon),
                contentDescription = null,
                modifier = Modifier.size(64.dp)
            )
            Spacer(Modifier.height(10.dp))
            Text(strings.drop, color = Ink, style = MaterialTheme.typography.bodyLarge)
            Spacer(Modifier.height(6.dp))
            Text(strings.dropHint, color = InkFaint, style = MaterialTheme.typography.bodySmall)
            Text(strings.multiHint, color = InkFaint, style = MaterialTheme.typography.bodySmall)
        }

        // 文件列表
        if (vm.items.isNotEmpty()) {
            Spacer(Modifier.height(16.dp))
            Column(
                modifier = Modifier
                    .fillMaxWidth()
                    .border(1.dp, Line, RoundedCornerShape(8.dp))
            ) {
                vm.items.forEach { item ->
                    FileRow(
                        item = item,
                        vm = vm,
                        onExport = {
                            exportTarget = item
                            val outName = item.name.substringBeforeLast('.') + "." + vm.outputFormat
                            exportOneLauncher?.launch(outName)
                        }
                    )
                    if (item != vm.items.last()) {
                        Spacer(
                            Modifier
                                .fillMaxWidth()
                                .padding(horizontal = 16.dp)
                                .height(1.dp)
                                .background(Line)
                        )
                    }
                }
            }
        }

        // 错误提示
        if (vm.coreError.isNotBlank()) {
            Spacer(Modifier.height(12.dp))
            Text(vm.coreError, color = Err, style = MaterialTheme.typography.bodySmall)
        }

        // 输出格式网格
        Spacer(Modifier.height(24.dp))
        Text(strings.format, color = InkSoft, style = MaterialTheme.typography.bodySmall, fontWeight = FontWeight.SemiBold)
        Spacer(Modifier.height(8.dp))
        formatExts.chunked(2).forEach { rowExts ->
            Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.spacedBy(10.dp)) {
                rowExts.forEach { ext ->
                    FormatCard(
                        ext = ext,
                        desc = strings.fmtDesc[ext] ?: "",
                        selected = vm.outputFormat == ext,
                        onClick = { vm.outputFormat = ext },
                        modifier = Modifier.weight(1f)
                    )
                }
                if (rowExts.size == 1) Spacer(Modifier.weight(1f))
            }
            Spacer(Modifier.height(10.dp))
        }

        // 参数行
        Spacer(Modifier.height(8.dp))
        SelectField(
            label = strings.bitrate,
            options = bitrateOptions(strings),
            selected = vm.bitrate,
            onSelect = { vm.bitrate = it }
        )
        Spacer(Modifier.height(10.dp))
        SelectField(
            label = strings.sampleRate,
            options = sampleRateOptions(strings),
            selected = vm.sampleRate,
            onSelect = { vm.sampleRate = it }
        )
        Spacer(Modifier.height(10.dp))
        SelectField(
            label = strings.channels,
            options = channelOptions(strings),
            selected = vm.channels,
            onSelect = { vm.channels = it }
        )

        // 操作
        Spacer(Modifier.height(24.dp))
        Row(
            Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Button(
                onClick = { vm.convertAll(context) },
                enabled = !vm.converting && vm.items.isNotEmpty(),
                colors = ButtonDefaults.buttonColors(containerColor = Accent, contentColor = Ink)
            ) {
                Text(if (vm.converting) strings.converting else strings.convertBtn, fontWeight = FontWeight.SemiBold)
            }
            OutlinedButton(
                onClick = { exportAllLauncher.launch(null) },
                enabled = vm.readyCount > 0
            ) {
                Text(strings.downloadAllCount(vm.readyCount))
            }
            Spacer(Modifier.weight(1f))
            TextButton(onClick = { vm.clearAll() }, enabled = !vm.converting) {
                Text(strings.clear, color = InkFaint)
            }
        }
    }
}

@Composable
private fun FormatCard(ext: String, desc: String, selected: Boolean, onClick: () -> Unit, modifier: Modifier = Modifier) {
    val bg = if (selected) AccentSoft else Paper
    val borderColor = if (selected) Accent else LineStrong
    Column(
        modifier = modifier
            .background(bg, RoundedCornerShape(8.dp))
            .border(1.dp, borderColor, RoundedCornerShape(8.dp))
            .clickable(onClick = onClick)
            .padding(12.dp)
    ) {
        Text(ext.uppercase(), color = Ink, fontSize = 18.sp, fontWeight = FontWeight.Bold, letterSpacing = 0.02.sp)
        Spacer(Modifier.height(2.dp))
        Text(desc, color = InkSoft, fontSize = 12.sp)
    }
}

@Composable
private fun FileRow(item: FileItem, vm: ConverterViewModel, onExport: () -> Unit) {
    val strings = LocalStrings.current
    Column(Modifier.fillMaxWidth().padding(horizontal = 16.dp, vertical = 12.dp)) {
        Row(verticalAlignment = Alignment.CenterVertically) {
            Column(Modifier.weight(1f)) {
                Text(
                    item.name,
                    color = Ink,
                    style = MaterialTheme.typography.bodyMedium,
                    fontWeight = FontWeight.SemiBold,
                    maxLines = 1,
                    overflow = TextOverflow.Ellipsis
                )
                Text(fmtSize(item.size), color = InkFaint, fontSize = 12.sp)
            }
            Spacer(Modifier.width(10.dp))
            when (item.status) {
                Status.CONVERTING -> {
                    LinearProgressIndicator(
                        progress = { item.progress / 100f },
                        modifier = Modifier.width(90.dp).height(6.dp),
                        color = Accent,
                        trackColor = Line
                    )
                    Spacer(Modifier.width(8.dp))
                    Text("${item.progress}%", color = InkSoft, fontSize = 12.sp)
                }
                Status.DONE -> Tag("✓ ${strings.statusDone}", ok = true)
                Status.ERROR -> Tag("✕ ${strings.statusError}", err = true)
                Status.IDLE -> Tag(strings.statusReady, idle = true)
            }
        }
        if (item.status == Status.ERROR && item.err.isNotBlank()) {
            Spacer(Modifier.height(4.dp))
            Text(item.err, color = Err, fontSize = 11.sp, maxLines = 2, overflow = TextOverflow.Ellipsis)
        }
        Spacer(Modifier.height(8.dp))
        Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            if (item.status == Status.DONE && item.outFile != null) {
                OutlinedButton(onClick = onExport) {
                    Text(strings.downloadOne)
                }
            }
            TextButton(onClick = { vm.removeItem(item.id) }, enabled = !vm.converting) {
                Text(strings.remove, color = InkFaint)
            }
        }
    }
}

@Composable
private fun Tag(text: String, ok: Boolean = false, err: Boolean = false, idle: Boolean = false) {
    val bg = when { ok -> OkSoft; err -> ErrSoft; else -> AccentSoft }
    val fg = when { ok -> Ok; err -> Err; else -> AccentDeep }
    Text(
        text,
        color = fg,
        fontSize = 12.sp,
        modifier = Modifier
            .background(bg, RoundedCornerShape(6.dp))
            .padding(horizontal = 8.dp, vertical = 3.dp)
    )
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
private fun SelectField(
    label: String,
    options: List<Pair<String, String>>,
    selected: String,
    onSelect: (String) -> Unit
) {
    var expanded by remember { mutableStateOf(false) }
    val currentLabel = options.firstOrNull { it.first == selected }?.second ?: selected
    ExposedDropdownMenuBox(expanded = expanded, onExpandedChange = { expanded = it }) {
        OutlinedTextField(
            value = currentLabel,
            onValueChange = {},
            readOnly = true,
            label = { Text(label) },
            trailingIcon = { ExposedDropdownMenuDefaults.TrailingIcon(expanded = expanded) },
            colors = OutlinedTextFieldDefaults.colors(
                focusedBorderColor = Accent,
                unfocusedBorderColor = LineStrong,
                focusedContainerColor = Paper,
                unfocusedContainerColor = Paper,
                focusedLabelColor = Accent,
                unfocusedLabelColor = InkSoft
            ),
            modifier = Modifier
                .fillMaxWidth()
                .menuAnchor(ExposedDropdownMenuAnchorType.PrimaryNotEditable)
        )
        ExposedDropdownMenu(expanded = expanded, onDismissRequest = { expanded = false }) {
            options.forEach { (value, optLabel) ->
                DropdownMenuItem(
                    text = { Text(optLabel, color = if (value == selected) AccentDeep else Ink) },
                    onClick = { onSelect(value); expanded = false }
                )
            }
        }
    }
}

private fun bitrateOptions(strings: Strings): List<Pair<String, String>> =
    listOf("auto" to strings.auto) + listOf(96, 128, 160, 192, 256, 320).map { it.toString() to "${it} kbps" }

private fun sampleRateOptions(strings: Strings): List<Pair<String, String>> =
    listOf("auto" to strings.auto) +
        listOf(8000, 11025, 16000, 22050, 44100, 48000, 96000).map { it.toString() to "${it} Hz" }

private fun channelOptions(strings: Strings): List<Pair<String, String>> =
    listOf(
        "auto" to strings.auto,
        "stereo" to strings.stereo,
        "mono" to strings.mono
    )

private fun queryName(context: android.content.Context, uri: android.net.Uri): String {
    context.contentResolver.query(uri, arrayOf(OpenableColumns.DISPLAY_NAME), null, null, null)?.use { c ->
        if (c.moveToFirst()) {
            val idx = c.getColumnIndex(OpenableColumns.DISPLAY_NAME)
            if (idx >= 0) return c.getString(idx) ?: "audio"
        }
    }
    return uri.lastPathSegment?.substringAfterLast('/') ?: "audio"
}

private fun querySize(context: android.content.Context, uri: android.net.Uri): Long {
    context.contentResolver.query(uri, arrayOf(OpenableColumns.SIZE), null, null, null)?.use { c ->
        if (c.moveToFirst()) {
            val idx = c.getColumnIndex(OpenableColumns.SIZE)
            if (idx >= 0 && !c.isNull(idx)) return c.getLong(idx)
        }
    }
    return 0L
}

private fun fmtSize(bytes: Long): String {
    if (bytes <= 0) return ""
    val kb = 1024.0
    val mb = 1024.0 * 1024
    return when {
        bytes < kb -> "$bytes B"
        bytes < mb -> "%.1f KB".format(bytes / kb)
        else -> "%.1f MB".format(bytes / mb)
    }
}
