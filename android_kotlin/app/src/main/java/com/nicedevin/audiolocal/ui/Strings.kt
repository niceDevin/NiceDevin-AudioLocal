package com.nicedevin.audiolocal.ui

import androidx.compose.runtime.staticCompositionLocalOf

enum class Lang { ZH, EN }

/** 应用内全部文案（对应网页版 locales/zh.js 与 en.js）。 */
data class Strings(
    val langSwitch: String,
    val brand: String,
    val footerMadeWith: String,
    val footerRights: String,

    val badge: String,
    val titlePrefix: String,
    val titleHighlight: String,
    val sub: String,
    val cta: String,
    val privacyPoint: String,

    val panelTitle: String,
    val drop: String,
    val dropHint: String,
    val multiHint: String,

    val format: String,
    val bitrate: String,
    val sampleRate: String,
    val channels: String,
    val auto: String,
    val stereo: String,
    val mono: String,

    val convertBtn: String,
    val converting: String,
    val downloadAll: String,
    val downloadOne: String,
    val clear: String,
    val remove: String,

    val statusReady: String,
    val statusDone: String,
    val statusError: String,
    val statusLoading: String,

    val fmtDesc: Map<String, String>,
    val multiNote: (Int) -> String,
    val downloadAllCount: (Int) -> String,

    val formatsTitle: String,
    val formatsSubtitle: String,
    val formatsInputTitle: String,
    val formatsOutputTitle: String,
    val formatsInput: List<String>,
    val formatsOutput: List<String>,
    val formatsNote: String,

    val howTitle: String,
    val howStep1Title: String,
    val howStep1Text: String,
    val howStep2Title: String,
    val howStep2Text: String,
    val howStep3Title: String,
    val howStep3Text: String,
    val howPrivacy: String,

    val privacyTitle: String,
    val privacyP1: String,
    val privacyP3: String
)

private val zh = Strings(
    langSwitch = "English",
    brand = "NiceDevin AudioLocal",
    footerMadeWith = "本地转码 · 无服务器",
    footerRights = "保留所有权利",

    badge = "100% 本地处理 · 全程在设备内完成",
    titlePrefix = "音频转换，",
    titleHighlight = "全程留在你的设备里。",
    sub = "拖入任意音频，选好目标格式，剩下的在设备里本地完成——文件不会离开这台设备。",
    cta = "开始转换",
    privacyPoint = "全程在本地执行",

    panelTitle = "音频转换器",
    drop = "点击选择音频文件",
    dropHint = "支持 MP3 · FLAC · WAV · M4A · OGG · Opus · AAC 等",
    multiHint = "可多选文件批量转换",

    format = "输出格式",
    bitrate = "比特率",
    sampleRate = "采样率",
    channels = "声道",
    auto = "自动",
    stereo = "立体声",
    mono = "单声道",

    convertBtn = "开始转换",
    converting = "转换中…",
    downloadAll = "全部下载",
    downloadOne = "下载",
    clear = "清空",
    remove = "移除",

    statusReady = "就绪",
    statusDone = "转换完成",
    statusError = "转换失败",
    statusLoading = "正在加载转码引擎…",

    fmtDesc = mapOf(
        "mp3" to "MP3 · 通用兼容",
        "flac" to "FLAC · 无损",
        "wav" to "WAV · 无压缩",
        "m4a" to "M4A · AAC 压缩",
        "ogg" to "OGG · Vorbis",
        "opus" to "Opus · 高效压缩"
    ),
    multiNote = { n -> "共 $n 个文件待转换" },
    downloadAllCount = { n -> "全部下载 ($n)" },

    formatsTitle = "支持格式",
    formatsSubtitle = "同一套 FFmpeg 内核，三端一致",
    formatsInputTitle = "可读取并转换的输入",
    formatsOutputTitle = "可导出的输出格式",
    formatsInput = listOf("MP3", "FLAC", "WAV", "M4A (AAC)", "OGG (Vorbis)", "Opus", "AAC", "AMR", "APE (无损)", "WMA", "ALAC", "WV"),
    formatsOutput = listOf("MP3", "FLAC", "WAV", "M4A (AAC)", "OGG (Vorbis)", "Opus"),
    formatsNote = "安卓客户端与网页版共用同一套 FFmpeg 内核，转码结果完全一致。",

    howTitle = "使用说明",
    howStep1Title = "选择文件",
    howStep1Text = "点击面板添加音频文件。支持多文件批量添加。",
    howStep2Title = "设置输出",
    howStep2Text = "挑选目标格式，并按需调整比特率、采样率与声道。",
    howStep3Title = "转换并下载",
    howStep3Text = "点击开始转换，进度实时显示，完成后可保存到设备。",
    howPrivacy = "所有处理都发生在你的设备上，没有任何文件被上传到服务器。",

    privacyTitle = "隐私与本地处理",
    privacyP1 = "这个转换器不需要服务器。所有处理都在你自己的设备上完成，你的音频文件从头到尾都不会离开这台设备。",
    privacyP3 = "全部本地转码，不上传任何文件。"
)

private val en = Strings(
    langSwitch = "中文",
    brand = "NiceDevin AudioLocal",
    footerMadeWith = "Local conversion · No server",
    footerRights = "All rights reserved",

    badge = "100% local processing · done entirely on your device",
    titlePrefix = "Audio conversion, ",
    titleHighlight = "entirely on your device.",
    sub = "Pick any audio, choose a target format — everything happens locally on this device. Nothing leaves it.",
    cta = "Start converting",
    privacyPoint = "Runs entirely locally",

    panelTitle = "Audio Converter",
    drop = "Tap to pick audio files",
    dropHint = "MP3 · FLAC · WAV · M4A · OGG · Opus · AAC supported",
    multiHint = "Select multiple files for batch conversion",

    format = "Output format",
    bitrate = "Bitrate",
    sampleRate = "Sample rate",
    channels = "Channels",
    auto = "Auto",
    stereo = "Stereo",
    mono = "Mono",

    convertBtn = "Start converting",
    converting = "Converting…",
    downloadAll = "Download all",
    downloadOne = "Download",
    clear = "Clear",
    remove = "Remove",

    statusReady = "Ready",
    statusDone = "Done",
    statusError = "Conversion failed",
    statusLoading = "Loading converter engine…",

    fmtDesc = mapOf(
        "mp3" to "MP3 · Universal",
        "flac" to "FLAC · Lossless",
        "wav" to "WAV · Uncompressed",
        "m4a" to "M4A · AAC",
        "ogg" to "OGG · Vorbis",
        "opus" to "Opus · Efficient"
    ),
    multiNote = { n -> "$n files ready" },
    downloadAllCount = { n -> "Download all ($n)" },

    formatsTitle = "Supported formats",
    formatsSubtitle = "One FFmpeg core, consistent across all platforms",
    formatsInputTitle = "Readable inputs",
    formatsOutputTitle = "Exportable outputs",
    formatsInput = listOf("MP3", "FLAC", "WAV", "M4A (AAC)", "OGG (Vorbis)", "Opus", "AAC", "AMR", "APE (lossless)", "WMA", "ALAC", "WV"),
    formatsOutput = listOf("MP3", "FLAC", "WAV", "M4A (AAC)", "OGG (Vorbis)", "Opus"),
    formatsNote = "Android and web share the same FFmpeg core — identical conversion results.",

    howTitle = "How it works",
    howStep1Title = "Select files",
    howStep1Text = "Tap the panel to add audio files. Batch add supported.",
    howStep2Title = "Set output",
    howStep2Text = "Pick the target format, adjust bitrate, sample rate and channels.",
    howStep3Title = "Convert & save",
    howStep3Text = "Tap start, watch live progress, then save to your device.",
    howPrivacy = "Everything runs on your device — no file is ever uploaded to a server.",

    privacyTitle = "Privacy & local processing",
    privacyP1 = "This converter needs no server. Everything runs on your own device — your audio never leaves this device.",
    privacyP3 = "Fully local conversion, nothing uploaded."
)

fun stringsFor(lang: Lang): Strings = if (lang == Lang.ZH) zh else en

val LocalStrings = staticCompositionLocalOf { zh }
