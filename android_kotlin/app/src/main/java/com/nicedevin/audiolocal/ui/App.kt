package com.nicedevin.audiolocal.ui

import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.statusBarsPadding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.rememberLazyListState
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.CompositionLocalProvider
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp
import com.nicedevin.audiolocal.R
import androidx.lifecycle.viewmodel.compose.viewModel
import com.nicedevin.audiolocal.ui.theme.Accent
import com.nicedevin.audiolocal.ui.theme.AccentSoft
import com.nicedevin.audiolocal.ui.theme.Ink
import com.nicedevin.audiolocal.ui.theme.InkFaint
import com.nicedevin.audiolocal.ui.theme.Paper
import com.nicedevin.audiolocal.ui.theme.Surface

@Composable
fun App() {
    val vm: ConverterViewModel = viewModel()
    var lang by rememberSaveable { mutableStateOf(Lang.ZH) }
    val strings = stringsFor(lang)

    CompositionLocalProvider(LocalStrings provides strings) {
        AudioLocalRoot(
            vm = vm,
            lang = lang,
            onToggleLang = { lang = if (lang == Lang.ZH) Lang.EN else Lang.ZH }
        )
    }
}

@Composable
private fun AudioLocalRoot(
    vm: ConverterViewModel,
    lang: Lang,
    onToggleLang: () -> Unit
) {
    val listState = rememberLazyListState()
    val strings = LocalStrings.current

    // LazyColumn 条目索引：Hero=0, Converter=1（供“开始转换”滚动定位）
    Box(Modifier.fillMaxSize().background(Paper)) {
        Column(Modifier.fillMaxSize()) {
            TopBar(lang, onToggleLang)
            LazyColumn(
                state = listState,
                modifier = Modifier.fillMaxWidth().padding(bottom = 16.dp)
            ) {
                item(key = "hero") { HeroSection(listState) }
                item(key = "converter") { ConverterPanel(vm) }
                item(key = "formats") { FormatsSection() }
                item(key = "how") { HowSection() }
                item(key = "privacy") { PrivacySection() }
                item(key = "footer") { Footer() }
            }
        }
    }
}

@Composable
private fun TopBar(lang: Lang, onToggleLang: () -> Unit) {
    val strings = LocalStrings.current
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .statusBarsPadding()
            .background(Surface)
            .padding(horizontal = 16.dp, vertical = 12.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        // 品牌图标（用户自己的 web 图标）
        Image(
            painter = painterResource(R.drawable.audiolocal_icon),
            contentDescription = null,
            modifier = Modifier.size(24.dp)
        )
        Spacer(Modifier.width(10.dp))
        Text(
            text = strings.brand,
            color = Ink,
            style = MaterialTheme.typography.titleMedium
        )
        Spacer(Modifier.weight(1f))

        TextButton(onClick = onToggleLang) {
            Text(text = strings.langSwitch, color = Accent)
        }
    }
}
