#include "pages.h"
#include "style.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>

static QLabel* makeSectionTitle(const QString& text) {
    auto* l = new QLabel(text);
    l->setObjectName("SecTitle");
    return l;
}
static QLabel* makeSectionSub(const QString& text) {
    auto* l = new QLabel(text);
    l->setObjectName("SecSub");
    l->setWordWrap(true);
    return l;
}
static QLabel* makeBody(const QString& text) {
    auto* l = new QLabel(text);
    l->setWordWrap(true);
    l->setStyleSheet(QString("color: %1; font-size: 14px;").arg(Style::InkSoft));
    return l;
}

// ============ FormatsPage ============
FormatsPage::FormatsPage(QWidget* parent) : QWidget(parent) {
    m_title = makeSectionTitle(QString());
    m_sub = makeSectionSub(QString());
    m_root = new QVBoxLayout(this);
    m_root->setContentsMargins(40, 36, 40, 40);
    m_root->setSpacing(14);
    m_root->addWidget(m_title);
    m_root->addWidget(m_sub);
    m_root->addSpacing(18);

    auto* columns = new QHBoxLayout;
    columns->setSpacing(40);

    // 输入列
    auto* inCol = new QVBoxLayout;
    m_inTitle = new QLabel(QString());
    m_inTitle->setStyleSheet(QString("font-size:15px; font-weight:600; color:%1;").arg(Style::Ink));
    inCol->addWidget(m_inTitle);
    const QStringList inList = {
        "MP3", "FLAC", "WAV", "M4A (AAC)", "OGG (Vorbis)", "Opus",
        "AAC", "AMR", "APE (无损)", "WMA", "ALAC", "WV"
    };
    for (const QString& f : inList) {
        auto* chip = new QLabel(f);
        chip->setStyleSheet(QString("background:%1; border:1px solid %2; border-radius:6px; padding:6px 12px; font-size:13px;")
                            .arg(Style::Paper, Style::LineStrong));
        inCol->addWidget(chip);
    }
    inCol->addStretch();

    // 输出列
    auto* outCol = new QVBoxLayout;
    m_outTitle = new QLabel(QString());
    m_outTitle->setStyleSheet(QString("font-size:15px; font-weight:600; color:%1;").arg(Style::Ink));
    outCol->addWidget(m_outTitle);
    const QStringList outList = { "MP3", "FLAC", "WAV", "M4A (AAC)", "OGG (Vorbis)", "Opus" };
    for (const QString& f : outList) {
        auto* chip = new QLabel(f);
        chip->setStyleSheet(QString("background:%1; border:1px solid %2; border-radius:6px; padding:6px 12px; font-size:13px; color:%3;")
                            .arg(Style::AccentSoft, Style::Accent, Style::AccentDeep));
        outCol->addWidget(chip);
    }
    outCol->addStretch();

    columns->addLayout(inCol);
    columns->addLayout(outCol);
    m_root->addLayout(columns);

    m_note = new QLabel(QString());
    m_note->setStyleSheet(QString("color:%1; font-size:14px; margin-top:10px;").arg(Style::InkFaint));
    m_root->addWidget(m_note);
    m_root->addStretch();

    retranslate();
}

void FormatsPage::retranslate() {
    if (m_en) {
        m_title->setText(QStringLiteral("Supported Formats"));
        m_sub->setText(QStringLiteral("One FFmpeg core across all three platforms"));
        m_inTitle->setText(QStringLiteral("Readable input formats"));
        m_outTitle->setText(QStringLiteral("Exportable output formats"));
        m_note->setText(QStringLiteral("Windows and Android clients are coming soon, with identical conversion results."));
    } else {
        m_title->setText(QStringLiteral("支持格式"));
        m_sub->setText(QStringLiteral("同一套 FFmpeg 内核，三端一致"));
        m_inTitle->setText(QStringLiteral("可读取并转换的输入"));
        m_outTitle->setText(QStringLiteral("可导出的输出格式"));
        m_note->setText(QStringLiteral("即将推出 Windows 与安卓客户端，转码结果与网页版完全一致。"));
    }
}

void FormatsPage::setLang(bool en) {
    m_en = en;
    retranslate();
}

// ============ HowPage ============
HowPage::HowPage(QWidget* parent) : QWidget(parent) {
    m_title = makeSectionTitle(QString());
    m_root = new QVBoxLayout(this);
    m_root->setContentsMargins(40, 36, 40, 40);
    m_root->setSpacing(14);
    m_root->addWidget(m_title);

    struct StepSpec { QLabel** title; QLabel** text; };
    StepSpec specs[3] = {
        { &m_step1Title, &m_step1Text },
        { &m_step2Title, &m_step2Text },
        { &m_step3Title, &m_step3Text }
    };
    for (int i = 0; i < 3; ++i) {
        auto* row = new QHBoxLayout;
        row->setSpacing(12);
        auto* num = new QLabel(QStringLiteral("0%1").arg(i + 1));
        num->setObjectName("StepNum");
        auto* body = new QVBoxLayout;
        *specs[i].title = new QLabel(QString());
        (*specs[i].title)->setStyleSheet("font-size:17px; font-weight:600;");
        *specs[i].text = makeBody(QString());
        body->addWidget(*specs[i].title);
        body->addWidget(*specs[i].text);
        row->addWidget(num, 0, Qt::AlignTop);
        row->addLayout(body);
        m_root->addLayout(row);
        m_root->addSpacing(8);
    }

    m_privacy = new QLabel(QString());
    m_privacy->setWordWrap(true);
    m_privacy->setStyleSheet(QString("color:%1; font-size:14px; border-left:3px solid %2; padding-left:14px;")
                             .arg(Style::InkSoft, Style::Accent));
    m_root->addSpacing(8);
    m_root->addWidget(m_privacy);
    m_root->addStretch();

    retranslate();
}

void HowPage::retranslate() {
    if (m_en) {
        m_title->setText(QStringLiteral("How it works"));
        m_step1Title->setText(QStringLiteral("Choose files"));
        m_step1Text->setText(QStringLiteral("Drag audio into the window, or click \u201cBrowse\u201d. Multiple files are supported."));
        m_step2Title->setText(QStringLiteral("Set the output"));
        m_step2Text->setText(QStringLiteral("Pick a target format and adjust bitrate, sample rate and channels as needed."));
        m_step3Title->setText(QStringLiteral("Convert & open"));
        m_step3Text->setText(QStringLiteral("Click Start converting — progress is shown live, then open the output folder."));
        m_privacy->setText(QStringLiteral("Everything is processed on your device; no file is ever uploaded to a server."));
    } else {
        m_title->setText(QStringLiteral("使用说明"));
        m_step1Title->setText(QStringLiteral("选择文件"));
        m_step1Text->setText(QStringLiteral("把音频拖进页面，或点击“浏览选择”。支持多文件批量添加。"));
        m_step2Title->setText(QStringLiteral("设置输出"));
        m_step2Text->setText(QStringLiteral("挑选目标格式，并按需调整比特率、采样率与声道。"));
        m_step3Title->setText(QStringLiteral("转换并下载"));
        m_step3Text->setText(QStringLiteral("点击开始转换，进度实时显示，完成后一键打开输出目录。"));
        m_privacy->setText(QStringLiteral("所有处理都发生在你的设备上，没有任何文件被上传到服务器。"));
    }
}

void HowPage::setLang(bool en) {
    m_en = en;
    retranslate();
}

// ============ PrivacyPage ============
PrivacyPage::PrivacyPage(QWidget* parent) : QWidget(parent) {
    m_title = makeSectionTitle(QString());
    m_p1 = makeBody(QString());
    m_p3 = makeBody(QString());

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(40, 36, 40, 40);
    root->setSpacing(14);
    root->addWidget(m_title);
    root->addWidget(m_p1);
    root->addSpacing(10);
    root->addWidget(m_p3);
    root->addStretch();

    retranslate();
}

void PrivacyPage::retranslate() {
    if (m_en) {
        m_title->setText(QStringLiteral("Privacy & local processing"));
        m_p1->setText(QStringLiteral("This converter needs no server. Everything happens on your own device, and your audio files never leave it from start to finish."));
        m_p3->setText(QStringLiteral("The Windows and Android clients follow the same principle: fully local conversion, nothing uploaded."));
    } else {
        m_title->setText(QStringLiteral("隐私与本地处理"));
        m_p1->setText(QStringLiteral("这个转换器不需要服务器。所有处理都在你自己的设备上完成，你的音频文件从头到尾都不会离开这台设备。"));
        m_p3->setText(QStringLiteral("这也是 Windows 与安卓客户端将要遵循的同一原则：全部本地转码，不上传。"));
    }
}

void PrivacyPage::setLang(bool en) {
    m_en = en;
    retranslate();
}
