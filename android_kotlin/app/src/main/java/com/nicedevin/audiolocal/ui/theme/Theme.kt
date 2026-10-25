package com.nicedevin.audiolocal.ui.theme

import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Typography
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color

// ===== 设计 Token：与网页版 base.css 一致 =====
val Paper = Color(0xFFF4F1EA)        // 页面底色：暖白纸
val Surface = Color(0xFFFBFAF6)      // 卡片/面板
val Ink = Color(0xFF1C1A16)          // 主文字：墨黑偏暖
val InkSoft = Color(0xFF55503F)      // 次级文字
val InkFaint = Color(0xFF8B8570)     // 弱文字/占位
val Line = Color(0xFFDDD6C6)         // 发丝边框
val LineStrong = Color(0xFFC9C1AC)

val Accent = Color(0xFFC2571A)       // 主强调：铜橙
val AccentDeep = Color(0xFF9A430F)
val AccentSoft = Color(0xFFF6E3D2)   // 强调的浅底
val Wave = Color(0xFF2B6A5E)         // 音波辅助绿

val Ok = Color(0xFF3A7D44)
val OkSoft = Color(0xFFE6EFE6)
val Err = Color(0xFFB03A2E)
val ErrSoft = Color(0xFFF4E2DF)

private val AudioColors = lightColorScheme(
    primary = Accent,
    onPrimary = Ink,
    primaryContainer = AccentSoft,
    onPrimaryContainer = AccentDeep,
    secondary = Wave,
    onSecondary = Color.White,
    background = Paper,
    onBackground = Ink,
    surface = Surface,
    onSurface = Ink,
    surfaceVariant = Surface,
    onSurfaceVariant = InkSoft,
    outline = LineStrong,
    outlineVariant = Line,
    error = Err,
    onError = Color.White,
    errorContainer = ErrSoft,
    onErrorContainer = Err,
    tertiary = Ok,
    onTertiary = Color.White
)

@Composable
fun AudioLocalTheme(content: @Composable () -> Unit) {
    MaterialTheme(
        colorScheme = AudioColors,
        typography = Typography(),
        content = content
    )
}
