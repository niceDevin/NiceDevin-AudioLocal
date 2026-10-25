package com.nicedevin.audiolocal.ui

import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.FlowRow
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyListState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.drawBehind
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import com.nicedevin.audiolocal.ui.theme.Accent
import com.nicedevin.audiolocal.ui.theme.AccentDeep
import com.nicedevin.audiolocal.ui.theme.AccentSoft
import com.nicedevin.audiolocal.ui.theme.Ink
import com.nicedevin.audiolocal.ui.theme.InkFaint
import com.nicedevin.audiolocal.ui.theme.InkSoft
import com.nicedevin.audiolocal.ui.theme.Line
import com.nicedevin.audiolocal.ui.theme.LineStrong
import com.nicedevin.audiolocal.ui.theme.Paper
import com.nicedevin.audiolocal.ui.theme.Surface
import kotlinx.coroutines.launch

private val Container = Modifier.fillMaxWidth().padding(horizontal = 20.dp)

@Composable
fun HeroSection(listState: LazyListState) {
    val strings = LocalStrings.current
    val scope = rememberCoroutineScope()
    Column(
        modifier = Container.padding(top = 28.dp, bottom = 24.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = strings.badge,
            color = InkSoft,
            style = MaterialTheme.typography.labelMedium,
            modifier = Modifier
                .background(Surface, RoundedCornerShape(50))
                .border(1.dp, LineStrong, RoundedCornerShape(50))
                .padding(horizontal = 16.dp, vertical = 6.dp)
        )
        Spacer(Modifier.height(18.dp))
        Text(
            text = strings.titlePrefix + strings.titleHighlight,
            color = Ink,
            style = MaterialTheme.typography.headlineMedium,
            fontWeight = FontWeight.Bold,
            textAlign = TextAlign.Center
        )
        Spacer(Modifier.height(12.dp))
        Text(
            text = strings.sub,
            color = InkSoft,
            style = MaterialTheme.typography.bodyLarge,
            textAlign = TextAlign.Center
        )
        Spacer(Modifier.height(20.dp))
        Button(
            onClick = { scope.launch { listState.animateScrollToItem(1) } },
            colors = ButtonDefaults.buttonColors(containerColor = Accent, contentColor = Ink)
        ) {
            Text(strings.cta, fontWeight = FontWeight.SemiBold)
        }
        Spacer(Modifier.height(14.dp))
        Text(
            text = strings.privacyPoint,
            color = InkFaint,
            style = MaterialTheme.typography.bodySmall
        )
    }
}

@Composable
private fun SectionTitle(title: String, subtitle: String? = null) {
    Column {
        Text(title, color = Ink, style = MaterialTheme.typography.headlineSmall, fontWeight = FontWeight.Bold)
        if (subtitle != null) {
            Spacer(Modifier.height(4.dp))
            Text(subtitle, color = InkSoft, style = MaterialTheme.typography.bodyMedium)
        }
    }
}

@Composable
fun FormatsSection() {
    val strings = LocalStrings.current
    Column(
        modifier = Container
            .background(Surface)
            .padding(vertical = 32.dp)
            .padding(horizontal = 20.dp)
    ) {
        SectionTitle(strings.formatsTitle, strings.formatsSubtitle)
        Spacer(Modifier.height(22.dp))

        Text(strings.formatsInputTitle, color = Ink, style = MaterialTheme.typography.titleMedium, fontWeight = FontWeight.SemiBold)
        Spacer(Modifier.height(10.dp))
        FlowRow(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            strings.formatsInput.forEach { f ->
                Chip(f, accent = false)
            }
        }

        Spacer(Modifier.height(24.dp))
        Text(strings.formatsOutputTitle, color = Ink, style = MaterialTheme.typography.titleMedium, fontWeight = FontWeight.SemiBold)
        Spacer(Modifier.height(10.dp))
        FlowRow(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            strings.formatsOutput.forEach { f ->
                Chip(f, accent = true)
            }
        }

        Spacer(Modifier.height(20.dp))
        Text(strings.formatsNote, color = InkFaint, style = MaterialTheme.typography.bodySmall)
    }
}

@Composable
private fun Chip(text: String, accent: Boolean) {
    val bg = if (accent) AccentSoft else Paper
    val borderColor = if (accent) Accent else LineStrong
    val fg = if (accent) AccentDeep else InkSoft
    Text(
        text = text,
        color = fg,
        style = MaterialTheme.typography.bodySmall,
        modifier = Modifier
            .background(bg, RoundedCornerShape(6.dp))
            .border(1.dp, borderColor, RoundedCornerShape(6.dp))
            .padding(horizontal = 12.dp, vertical = 6.dp)
    )
}

@Composable
fun HowSection() {
    val strings = LocalStrings.current
    Column(modifier = Container.padding(vertical = 32.dp)) {
        SectionTitle(strings.howTitle)
        Spacer(Modifier.height(8.dp))
        StepRow("01", strings.howStep1Title, strings.howStep1Text)
        StepRow("02", strings.howStep2Title, strings.howStep2Text)
        StepRow("03", strings.howStep3Title, strings.howStep3Text)
        Spacer(Modifier.height(14.dp))
        Text(
            text = strings.howPrivacy,
            color = InkSoft,
            style = MaterialTheme.typography.bodyMedium,
            modifier = Modifier
                .drawBehind {
                    drawLine(
                        color = Accent,
                        start = Offset(0f, 0f),
                        end = Offset(0f, size.height),
                        strokeWidth = 3.dp.toPx()
                    )
                }
                .padding(start = 14.dp)
        )
    }
}

@Composable
private fun StepRow(num: String, title: String, text: String) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 16.dp),
        verticalAlignment = Alignment.Top
    ) {
        Text(
            text = num,
            color = Accent,
            style = MaterialTheme.typography.bodyMedium,
            fontWeight = FontWeight.Bold,
            modifier = Modifier.width(40.dp)
        )
        Column {
            Text(title, color = Ink, style = MaterialTheme.typography.titleMedium, fontWeight = FontWeight.SemiBold)
            Spacer(Modifier.height(4.dp))
            Text(text, color = InkSoft, style = MaterialTheme.typography.bodyMedium)
        }
    }
}

@Composable
fun PrivacySection() {
    val strings = LocalStrings.current
    Column(
        modifier = Container
            .background(Surface)
            .padding(vertical = 32.dp)
            .padding(horizontal = 20.dp)
    ) {
        SectionTitle(strings.privacyTitle)
        Spacer(Modifier.height(14.dp))
        Text(strings.privacyP1, color = InkSoft, style = MaterialTheme.typography.bodyMedium)
        Spacer(Modifier.height(12.dp))
        Text(strings.privacyP3, color = InkSoft, style = MaterialTheme.typography.bodyMedium)
    }
}

@Composable
fun Footer() {
    val strings = LocalStrings.current
    Row(
        modifier = Container.padding(vertical = 20.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Text(strings.brand, color = Ink, style = MaterialTheme.typography.bodyMedium, fontWeight = FontWeight.Bold)
        Spacer(Modifier.width(16.dp))
        Text(strings.footerMadeWith, color = InkFaint, style = MaterialTheme.typography.bodySmall)
        Spacer(Modifier.weight(1f))
        Text(
            text = "© ${java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)} NiceDevin · ${strings.footerRights}",
            color = InkFaint,
            style = MaterialTheme.typography.bodySmall
        )
    }
}
