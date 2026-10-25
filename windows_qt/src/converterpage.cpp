#include "converterpage.h"
#include "style.h"

#include <QRegularExpression>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QProgressBar>
#include <QFrame>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QCoreApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QSizePolicy>

ConverterPage::ConverterPage(QWidget* parent) : QWidget(parent) {
    setAcceptDrops(true);   // 整页接收文件拖放，避免子控件拦截后无人处理
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(40, 28, 40, 40);
    root->setSpacing(18);

    // ===== Hero 区（对齐网页版）=====
    m_heroBadge = new QLabel(this);
    m_heroBadge->setObjectName("HeroBadge");
    m_heroBadge->setAlignment(Qt::AlignCenter);
    m_heroBadge->setFixedHeight(30);
    root->addWidget(m_heroBadge);

    m_heroTitle = new QLabel(this);
    m_heroTitle->setObjectName("HeroTitle");
    m_heroTitle->setAlignment(Qt::AlignCenter);
    m_heroTitle->setWordWrap(true);
    m_heroTitle->setTextFormat(Qt::RichText);
    root->addWidget(m_heroTitle);

    m_heroSub = new QLabel(this);
    m_heroSub->setObjectName("HeroSub");
    m_heroSub->setAlignment(Qt::AlignCenter);
    m_heroSub->setWordWrap(true);
    m_heroSub->setMaximumWidth(760);
    root->addWidget(m_heroSub, 0, Qt::AlignHCenter);

    m_heroPoint = new QLabel(this);
    m_heroPoint->setObjectName("HeroPoint");
    m_heroPoint->setAlignment(Qt::AlignCenter);
    root->addWidget(m_heroPoint);
    root->addSpacing(10);

    // ===== 面板容器 =====
    auto* panel = new QFrame(this);
    panel->setObjectName("Panel");

    auto* panelRoot = new QVBoxLayout(panel);
    panelRoot->setContentsMargins(28, 24, 28, 24);
    panelRoot->setSpacing(16);

    // 面板头
    auto* head = new QHBoxLayout;
    m_panelTitle = new QLabel(panel);
    m_panelTitle->setObjectName("PanelTitle");
    m_panelSub = new QLabel(panel);
    m_panelSub->setObjectName("PanelSub");
    head->addWidget(m_panelTitle);
    head->addStretch();
    head->addWidget(m_panelSub);
    panelRoot->addLayout(head);

    // ===== 拖放区 =====
    m_dropZone = new QFrame(panel);
    m_dropZone->setObjectName("DropZone");
    // 拖放统一由 ConverterPage 处理；这里不 setAcceptDrops，否则 QFrame 会拦截却没处理函数
    auto* dropLay = new QVBoxLayout(m_dropZone);
    dropLay->setContentsMargins(24, 28, 24, 28);
    dropLay->setSpacing(6);
    dropLay->setAlignment(Qt::AlignCenter);

    m_dropMain = new QLabel(m_dropZone);
    m_dropMain->setAlignment(Qt::AlignCenter);
    m_dropMain->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    m_browseBtn = new QPushButton(m_dropZone);
    m_browseBtn->setObjectName("BrowseLink");
    m_browseBtn->setCursor(Qt::PointingHandCursor);
    m_browseBtn->setMinimumHeight(30);
    m_browseBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(m_browseBtn, &QPushButton::clicked, this, &ConverterPage::pickInputFiles);

    auto* dropTop = new QHBoxLayout;
    dropTop->setSpacing(4);
    dropTop->addStretch();
    dropTop->addWidget(m_dropMain);
    dropTop->addWidget(m_browseBtn);
    dropTop->addStretch();
    dropLay->addLayout(dropTop);

    auto* hint1 = new QLabel(m_dropZone);
    hint1->setObjectName("DropHint");
    hint1->setAlignment(Qt::AlignCenter);
    dropLay->addWidget(hint1);

    auto* hint2 = new QLabel(m_dropZone);
    hint2->setObjectName("DropHint");
    hint2->setAlignment(Qt::AlignCenter);
    dropLay->addWidget(hint2);

    // 输入路径行
    auto* pathRow = new QHBoxLayout;
    pathRow->setSpacing(8);
    m_inputEdit = new QLineEdit(m_dropZone);
    m_inputEdit->setObjectName("PathInput");
    auto* addBtn = new QPushButton(m_dropZone);
    addBtn->setObjectName("BtnSecondary");
    addBtn->setMinimumWidth(88);
    addBtn->setMinimumHeight(38);
    pathRow->addWidget(m_inputEdit, 1);
    pathRow->addWidget(addBtn);
    dropLay->addLayout(pathRow);

    connect(addBtn, &QPushButton::clicked, this, &ConverterPage::addFromText);
    connect(m_inputEdit, &QLineEdit::returnPressed, this, &ConverterPage::addFromText);
    panelRoot->addWidget(m_dropZone);

    // ===== 输出目录行 =====
    auto* outRow = new QHBoxLayout;
    outRow->setSpacing(8);
    auto* outLabel = new QLabel(panel);
    outLabel->setObjectName("OptLabel");
    m_outputEdit = new QLineEdit(panel);
    m_outputEdit->setObjectName("OutputInput");
    auto* outBrowse = new QPushButton(panel);
    outBrowse->setObjectName("BtnSecondary");
    outBrowse->setMinimumHeight(38);
    m_openDirBtn = new QPushButton(panel);
    m_openDirBtn->setObjectName("BtnSecondary");
    m_openDirBtn->setMinimumHeight(38);
    m_openDirBtn->setEnabled(false);
    outRow->addWidget(outLabel);
    outRow->addWidget(m_outputEdit, 1);
    outRow->addWidget(outBrowse);
    outRow->addWidget(m_openDirBtn);
    panelRoot->addLayout(outRow);

    connect(outBrowse, &QPushButton::clicked, this, &ConverterPage::pickOutputDir);
    connect(m_openDirBtn, &QPushButton::clicked, this, &ConverterPage::openOutputDir);

    // ===== 错误提示 =====
    m_errorLabel = new QLabel(panel);
    m_errorLabel->setObjectName("ErrorNote");
    m_errorLabel->setWordWrap(true);
    m_errorLabel->hide();
    panelRoot->addWidget(m_errorLabel);

    // ===== 文件表 =====
    m_table = new QTableWidget(panel);
    m_table->setObjectName("FileTable");
    m_table->setColumnCount(4);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(false);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    // 状态列/操作列用固定宽度：ResizeToContents 遇到进度条控件和操作按钮会塌缩到看不见
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    m_table->setColumnWidth(2, 140);
    m_table->setColumnWidth(3, 150);
    m_table->setMinimumHeight(120);
    panelRoot->addWidget(m_table);

    // ===== 格式选项 =====
    auto* fmtLabel = new QLabel(panel);
    fmtLabel->setObjectName("OptLabel");
    panelRoot->addWidget(fmtLabel);

    m_formatCmb = new QComboBox(panel);
    m_formatCmb->setObjectName("CmbBox");
    const QStringList exts = { "mp3", "flac", "wav", "m4a", "ogg", "opus" };
    for (const QString& e : exts) m_formatCmb->addItem(e.toUpper(), e);
    panelRoot->addWidget(m_formatCmb);

    // 参数行
    auto* optRow = new QGridLayout;
    optRow->setSpacing(16);

    auto* brLabel = new QLabel(panel);
    brLabel->setObjectName("OptLabel");
    m_bitrateCmb = new QComboBox(panel);
    m_bitrateCmb->setObjectName("CmbBox");
    m_bitrateCmb->addItem(QString(), "auto");
    for (int b : {96, 128, 160, 192, 256, 320}) m_bitrateCmb->addItem(QString::number(b) + " kbps", QString::number(b));

    auto* srLabel = new QLabel(panel);
    srLabel->setObjectName("OptLabel");
    m_sampleCmb = new QComboBox(panel);
    m_sampleCmb->setObjectName("CmbBox");
    m_sampleCmb->addItem(QString(), "auto");
    for (int sr : {8000, 11025, 16000, 22050, 44100, 48000, 96000}) m_sampleCmb->addItem(QString::number(sr) + " Hz", QString::number(sr));

    auto* chLabel = new QLabel(panel);
    chLabel->setObjectName("OptLabel");
    m_channelCmb = new QComboBox(panel);
    m_channelCmb->setObjectName("CmbBox");
    m_channelCmb->addItem(QString(), "auto");
    m_channelCmb->addItem(QString(), "stereo");
    m_channelCmb->addItem(QString(), "mono");

    optRow->addWidget(brLabel, 0, 0);
    optRow->addWidget(m_bitrateCmb, 1, 0);
    optRow->addWidget(srLabel, 0, 1);
    optRow->addWidget(m_sampleCmb, 1, 1);
    optRow->addWidget(chLabel, 0, 2);
    optRow->addWidget(m_channelCmb, 1, 2);
    panelRoot->addLayout(optRow);

    // ===== 操作 =====
    auto* actRow = new QHBoxLayout;
    actRow->setSpacing(12);
    m_convertBtn = new QPushButton(panel);
    m_convertBtn->setObjectName("BtnPrimary");
    m_convertBtn->setMinimumHeight(44);
    m_convertBtn->setEnabled(false);
    // 转换成功后启用：一键打开导出目录
    m_exportBtn = new QPushButton(panel);
    m_exportBtn->setObjectName("BtnSecondary");
    m_exportBtn->setCursor(Qt::PointingHandCursor);
    m_exportBtn->setMinimumHeight(44);
    m_exportBtn->setEnabled(false);
    connect(m_exportBtn, &QPushButton::clicked, this, &ConverterPage::exportAll);
    auto* clearBtn = new QPushButton(panel);
    clearBtn->setObjectName("BtnGhost");
    clearBtn->setMinimumHeight(34);
    actRow->addWidget(m_convertBtn);
    actRow->addWidget(m_exportBtn);
    actRow->addStretch();
    actRow->addWidget(clearBtn);
    panelRoot->addLayout(actRow);

    connect(m_convertBtn, &QPushButton::clicked, this, &ConverterPage::startConvert);
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        if (m_worker.isRunning()) return;
        // 清空时删除尚未导出的临时区文件，并清理临时目录
        for (const FileItem& it : m_items) {
            if (!it.exported && !it.result.isEmpty()) QFile::remove(it.result);
        }
        if (!m_tempDir.isEmpty() && QFileInfo(m_tempDir).isDir()) {
            QDir(m_tempDir).removeRecursively();
            m_tempDir.clear();
        }
        m_items.clear();
        rebuildFileTable();
        m_convertBtn->setEnabled(false);
    });
    connect(m_table, &QTableWidget::cellClicked, this, [this](int row, int) {
        if (row >= 0 && row < m_items.size()) openItemDir(row);
    });

    // ===== ffmpeg worker =====
    const QString ffPath = QCoreApplication::applicationDirPath() + "/ffmpeg.exe";
    m_worker.setFfmpegPath(ffPath);
    connect(&m_worker, &FFmpegWorker::progressChanged, this, &ConverterPage::onProgress);
    connect(&m_worker, &FFmpegWorker::finishedOk, this, &ConverterPage::onConvertOk);
    connect(&m_worker, &FFmpegWorker::failed, this, &ConverterPage::onConvertFail);
    connect(&m_worker, &FFmpegWorker::cancelled, this, &ConverterPage::onCancelled);

    // 面板加入页面布局（hero 已在顶部）
    root->addWidget(panel);

    retranslate();
}

ConverterPage::~ConverterPage() {
    // 关闭时清理临时区，避免残留转换产物
    if (!m_tempDir.isEmpty() && QFileInfo(m_tempDir).isDir()) {
        QDir(m_tempDir).removeRecursively();
    }
}

void ConverterPage::retranslate() {
    if (m_en) {
        s_heroBadge = QStringLiteral("100% local · Everything stays on your device");
        s_heroTitlePre = QStringLiteral("Convert audio, ");
        s_heroTitleHi = QStringLiteral("right on your device.");
        s_heroSub = QStringLiteral("Drop in any audio, pick a target format, and the rest happens locally — your files never leave this device.");
        s_heroPoint = QStringLiteral("Processed entirely on-device");
        s_panelTitle = QStringLiteral("Audio Converter");
        s_drop = QStringLiteral("Drop audio files here, or");
        s_browse = QStringLiteral("Browse");
        s_hint = QStringLiteral("Supports MP3 · FLAC · WAV · M4A · OGG · Opus · AAC etc.");
        s_multi = QStringLiteral("Batch conversion supported");
        s_placeholder = QStringLiteral("Or type a file path (separate multiple with ;)");
        s_add = QStringLiteral("Add");
        s_outLabel = QStringLiteral("Output folder");
        s_outPlaceholder = QStringLiteral("Choose or type the output folder path");
        s_openDir = QStringLiteral("Browse");
        s_openOut = QStringLiteral("Open folder");
        s_convert = QStringLiteral("Start converting");
        s_converting = QStringLiteral("Converting…");
        s_done = QStringLiteral("Done");
        s_err = QStringLiteral("Failed");
        s_ready = QStringLiteral("Ready");
        s_unsupported = QStringLiteral("Unsupported file format: ");
        s_pathInvalid = QStringLiteral("Invalid path or unsupported format");
        s_dropFailed = QStringLiteral("Could not parse dropped files. Use Browse or type a path instead.");
        s_outRequired = QStringLiteral("Please choose an output folder first");
        s_clear = QStringLiteral("Clear");
        s_countNote = QStringLiteral("%1 file(s) pending");
        s_formatLabel = QStringLiteral("Output format");
        s_bitrateLabel = QStringLiteral("Bitrate");
        s_sampleLabel = QStringLiteral("Sample rate");
        s_chanLabel = QStringLiteral("Channels");
        s_auto = QStringLiteral("Auto");
        s_stereo = QStringLiteral("Stereo");
        s_mono = QStringLiteral("Mono");
        s_remove = QStringLiteral("Remove");
        s_errorTitle = QStringLiteral("Conversion failed");
        s_cancel = QStringLiteral("Cancel");
        s_export = QStringLiteral("Export");
        s_exported = QStringLiteral("Exported");
    } else {
        s_heroBadge = QStringLiteral("100% 本地处理 · 全程在设备内完成");
        s_heroTitlePre = QStringLiteral("音频转换，");
        s_heroTitleHi = QStringLiteral("全程留在你的设备里。");
        s_heroSub = QStringLiteral("拖入任意音频，选好目标格式，剩下的在本地完成——文件不会离开这台设备。");
        s_heroPoint = QStringLiteral("全程在本地执行");
        s_panelTitle = QStringLiteral("音频转换器");
        s_drop = QStringLiteral("拖放音频文件到这里，或");
        s_browse = QStringLiteral("浏览选择");
        s_hint = QStringLiteral("支持 MP3 · FLAC · WAV · M4A · OGG · Opus · AAC 等");
        s_multi = QStringLiteral("可多选文件批量转换");
        s_placeholder = QStringLiteral("或直接输入文件路径（多个用分号分隔）");
        s_add = QStringLiteral("添加");
        s_outLabel = QStringLiteral("导出目录");
        s_outPlaceholder = QStringLiteral("选择或输入输出文件夹路径");
        s_openDir = QStringLiteral("浏览");
        s_openOut = QStringLiteral("打开目录");
        s_convert = QStringLiteral("开始转换");
        s_converting = QStringLiteral("转换中…");
        s_done = QStringLiteral("完成");
        s_err = QStringLiteral("失败");
        s_ready = QStringLiteral("就绪");
        s_unsupported = QStringLiteral("以下文件格式不支持：");
        s_pathInvalid = QStringLiteral("路径无效或格式不支持");
        s_dropFailed = QStringLiteral("拖入文件解析失败，请改用浏览选择或直接输入路径");
        s_outRequired = QStringLiteral("请先选择导出目录");
        s_clear = QStringLiteral("清空");
        s_countNote = QStringLiteral("共 %1 个文件待转换");
        s_formatLabel = QStringLiteral("输出格式");
        s_bitrateLabel = QStringLiteral("比特率");
        s_sampleLabel = QStringLiteral("采样率");
        s_chanLabel = QStringLiteral("声道");
        s_auto = QStringLiteral("自动");
        s_stereo = QStringLiteral("立体声");
        s_mono = QStringLiteral("单声道");
        s_remove = QStringLiteral("删除");
        s_errorTitle = QStringLiteral("转换失败");
        s_cancel = QStringLiteral("取消");
        s_export = QStringLiteral("导出");
        s_exported = QStringLiteral("已导出");
    }

    m_heroBadge->setText(s_heroBadge);
    m_heroTitle->setText(QStringLiteral("%1<span style='color:%2;'>%3</span>")
                         .arg(s_heroTitlePre, Style::Accent, s_heroTitleHi));
    m_heroSub->setText(s_heroSub);
    m_heroPoint->setText(s_heroPoint);
    m_panelTitle->setText(s_panelTitle);

    // 动态组合文案
    m_dropMain->setText(s_drop);
    m_browseBtn->setText(s_browse);

    auto hints = m_dropZone->findChildren<QLabel*>("DropHint");
    if (hints.size() >= 2) {
        hints[0]->setText(s_hint);
        hints[1]->setText(s_multi);
    }
    m_inputEdit->setPlaceholderText(s_placeholder);
    auto btns = m_dropZone->findChildren<QPushButton*>();
    for (auto* b : btns) if (b->objectName() == "BtnSecondary") { b->setText(s_add); break; }

    // 输出行第一个 QLabel
    QList<QLabel*> labels = findChildren<QLabel*>("OptLabel");
    if (labels.size() > 0) labels[0]->setText(s_outLabel);
    m_outputEdit->setPlaceholderText(s_outPlaceholder);
    // 输出行按钮
    QList<QPushButton*> secBtns = findChildren<QPushButton*>("BtnSecondary");
    for (auto* b : secBtns) {
        if (b == m_exportBtn) { b->setText(s_export); continue; }   // 转换成功后打开的导出按钮
        if (b == m_openDirBtn) { b->setText(s_openOut); continue; }
        if (b->parent() == m_dropZone) continue;
        b->setText(s_openDir);
    }

    m_convertBtn->setText(m_worker.isRunning() ? s_converting : s_convert);
    clearBtnText();
    m_panelSub->setText(s_countNote.arg(m_items.size()));

    // 组合框文案
    for (int i = 0; i < m_bitrateCmb->count(); ++i) {
        if (m_bitrateCmb->itemData(i) == "auto") { m_bitrateCmb->setItemText(i, s_auto); break; }
    }
    for (int i = 0; i < m_sampleCmb->count(); ++i) {
        if (m_sampleCmb->itemData(i) == "auto") { m_sampleCmb->setItemText(i, s_auto); break; }
    }
    m_channelCmb->setItemText(0, s_auto);
    m_channelCmb->setItemText(1, s_stereo);
    m_channelCmb->setItemText(2, s_mono);

    if (labels.size() >= 4) {
        labels[1]->setText(s_formatLabel);
        labels[2]->setText(s_bitrateLabel);
        labels[3]->setText(s_sampleLabel);
    }
    if (labels.size() >= 5) labels[4]->setText(s_chanLabel);

    // 表头
    m_table->setHorizontalHeaderLabels({
        m_en ? QStringLiteral("File") : QStringLiteral("文件"),
        m_en ? QStringLiteral("Path") : QStringLiteral("路径"),
        m_en ? QStringLiteral("Status") : QStringLiteral("状态"),
        m_en ? QStringLiteral("Action") : QStringLiteral("操作")
    });

    rebuildFileTable();
}

void ConverterPage::setLang(bool en) {
    m_en = en;
    retranslate();
}

void ConverterPage::clearBtnText() {
    QList<QPushButton*> ghosts = findChildren<QPushButton*>("BtnGhost");
    for (auto* b : ghosts) b->setText(s_clear);
}

void ConverterPage::rebuildFileTable() {
    m_table->setRowCount(m_items.size());
    m_table->verticalHeader()->setDefaultSectionSize(40);
    for (int r = 0; r < m_items.size(); ++r) {
        m_table->setRowHeight(r, 40);
        const FileItem& it = m_items[r];
        auto* nameItem = new QTableWidgetItem(it.name);
        nameItem->setToolTip(it.path);
        auto* pathItem = new QTableWidgetItem(it.path);
        pathItem->setToolTip(it.path);
        QString statusText, statusColor, statusBg;
        switch (it.status) {
        case FileItem::Idle:       statusText = s_ready; statusColor = Style::AccentDeep; statusBg = Style::AccentSoft; break;
        case FileItem::Converting: statusText = QString::number(it.progress) + "%"; statusColor = Style::InkSoft; statusBg = Style::Line; break;
        case FileItem::Done:       statusText = "✓ " + (it.exported ? s_exported : s_done); statusColor = Style::Ok; statusBg = "#e6efe6"; break;
        case FileItem::Error:      statusText = "✕ " + s_err; statusColor = Style::Err; statusBg = "#f4e2df"; break;
        }
        auto* statusItem = new QTableWidgetItem(statusText);
        statusItem->setForeground(QColor(statusColor));
        statusItem->setBackground(QColor(statusBg));
        if (!it.err.isEmpty()) statusItem->setToolTip(it.err);
        if (it.status == FileItem::Converting) {
            auto* bar = new QProgressBar(m_table);
            bar->setObjectName("PBar");
            bar->setRange(0, 100);
            bar->setValue(it.progress);
            m_table->setCellWidget(r, 2, bar);
        }
        // 状态列文字（转换中时被上面的进度条控件覆盖显示，其余状态显示文本）
        m_table->setItem(r, 2, statusItem);

        // 操作列：Idle/Error=删除，Converting=取消，Done=导出+删除
        QWidget* actionCell = nullptr;
        if (it.status == FileItem::Converting) {
            auto* btn = new QPushButton(m_table);
            btn->setObjectName("TableBtn");
            btn->setCursor(Qt::PointingHandCursor);
            btn->setMinimumHeight(28);
            btn->setMinimumWidth(64);
            btn->setText(s_cancel);
            connect(btn, &QPushButton::clicked, this, [this]() {
                if (m_worker.isRunning()) m_worker.cancel();
            });
            actionCell = btn;
        } else if (it.status == FileItem::Done) {
            // 已完成：同时提供“导出”和“删除”两个按钮
            auto* cell = new QWidget(m_table);
            auto* h = new QHBoxLayout(cell);
            h->setContentsMargins(0, 0, 0, 0);
            h->setSpacing(4);
            auto* expBtn = new QPushButton(cell);
            expBtn->setObjectName("TableBtn");
            expBtn->setCursor(Qt::PointingHandCursor);
            expBtn->setMinimumHeight(28);
            expBtn->setText(s_export);
            connect(expBtn, &QPushButton::clicked, this, [this, r]() { exportItem(r); });
            auto* delBtn = new QPushButton(cell);
            delBtn->setObjectName("TableBtn");
            delBtn->setCursor(Qt::PointingHandCursor);
            delBtn->setMinimumHeight(28);
            delBtn->setText(s_remove);
            connect(delBtn, &QPushButton::clicked, this, [this, r]() { removeItem(r); });
            h->addWidget(expBtn);
            h->addWidget(delBtn);
            actionCell = cell;
        } else {
            auto* btn = new QPushButton(m_table);
            btn->setObjectName("TableBtn");
            btn->setCursor(Qt::PointingHandCursor);
            btn->setMinimumHeight(28);
            btn->setMinimumWidth(64);
            btn->setText(s_remove);
            connect(btn, &QPushButton::clicked, this, [this, r]() { removeItem(r); });
            actionCell = btn;
        }
        m_table->setCellWidget(r, 3, actionCell);

        m_table->setItem(r, 0, nameItem);
        m_table->setItem(r, 1, pathItem);
    }
    m_panelSub->setText(s_countNote.arg(m_items.size()));
    m_convertBtn->setEnabled(!m_items.isEmpty() && !m_worker.isRunning());
    // 有任一文件转换成功时，启用“导出”按钮
    bool anyNotExported = false;
    for (const FileItem& it : m_items) {
        if (it.status == FileItem::Done && !it.exported) { anyNotExported = true; break; }
    }
    m_exportBtn->setEnabled(anyNotExported);
}

void ConverterPage::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasUrls()) {
        m_dragging = true;
        m_dropZone->setProperty("dragActive", "true");
        m_dropZone->style()->unpolish(m_dropZone);
        m_dropZone->style()->polish(m_dropZone);
        e->acceptProposedAction();
    }
}

void ConverterPage::dragLeaveEvent(QDragLeaveEvent* e) {
    m_dragging = false;
    m_dropZone->setProperty("dragActive", "false");
    m_dropZone->style()->unpolish(m_dropZone);
    m_dropZone->style()->polish(m_dropZone);
    QWidget::dragLeaveEvent(e);
}

void ConverterPage::dropEvent(QDropEvent* e) {
    m_dragging = false;
    m_dropZone->setProperty("dragActive", "false");
    m_dropZone->style()->unpolish(m_dropZone);
    m_dropZone->style()->polish(m_dropZone);

    QStringList paths;
    const QList<QUrl> urls = e->mimeData()->urls();
    for (const QUrl& u : urls) {
        if (u.isLocalFile()) paths << u.toLocalFile();
    }
    int added = 0;
    for (const QString& p : paths) {
        const QString ext = QFileInfo(p).suffix().toLower();
        if (!acceptExt(ext)) continue;
        bool dup = false;
        for (const FileItem& it : m_items) if (it.path.compare(p, Qt::CaseInsensitive) == 0) { dup = true; break; }
        if (dup) continue;
        FileItem it;
        it.path = p;
        it.name = QFileInfo(p).fileName();
        m_items.append(it);
        ++added;
    }
    if (added == 0 && !paths.isEmpty()) setError(s_dropFailed);
    else if (added == 0 && paths.isEmpty()) setError(s_dropFailed);
    else clearError();
    rebuildFileTable();
    e->acceptProposedAction();
}

void ConverterPage::pickInputFiles() {
    const QStringList files = QFileDialog::getOpenFileNames(
        this, QStringLiteral("选择音频文件"), QDir::homePath(),
        QStringLiteral("音频文件 (*.mp3 *.flac *.wav *.m4a *.aac *.ogg *.opus *.amr *.wma *.ape *.wv);;所有文件 (*)"));
    int added = 0;
    for (const QString& p : files) {
        const QString ext = QFileInfo(p).suffix().toLower();
        if (!acceptExt(ext)) continue;
        bool dup = false;
        for (const FileItem& it : m_items) if (it.path.compare(p, Qt::CaseInsensitive) == 0) { dup = true; break; }
        if (dup) continue;
        FileItem it;
        it.path = p;
        it.name = QFileInfo(p).fileName();
        m_items.append(it);
        ++added;
    }
    if (added == 0 && !files.isEmpty()) setError(s_unsupported);
    else clearError();
    rebuildFileTable();
}

void ConverterPage::addFromText() {
    const QString txt = m_inputEdit->text().trimmed();
    if (txt.isEmpty()) return;
    const QStringList parts = txt.split(QRegularExpression("[;\\n]"), Qt::SkipEmptyParts);
    int added = 0;
    for (const QString& raw : parts) {
        QString p = raw.trimmed();
        if (p.startsWith('"')) p.remove(0, 1);
        if (p.endsWith('"')) p.chop(1);
        p = p.trimmed();
        if (p.isEmpty()) continue;
        const QString ext = QFileInfo(p).suffix().toLower();
        if (!acceptExt(ext)) { setError(s_unsupported + p); continue; }
        bool dup = false;
        for (const FileItem& it : m_items) if (it.path.compare(p, Qt::CaseInsensitive) == 0) { dup = true; break; }
        if (dup) continue;
        FileItem it;
        it.path = p;
        it.name = QFileInfo(p).fileName();
        m_items.append(it);
        ++added;
    }
    if (added == 0 && !txt.contains(';')) setError(s_pathInvalid);
    else clearError();
    m_inputEdit->clear();
    rebuildFileTable();
}

void ConverterPage::pickOutputDir() {
    const QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择导出目录"), m_outputDir.isEmpty() ? QDir::homePath() : m_outputDir);
    if (!dir.isEmpty()) {
        m_outputDir = dir;
        m_outputEdit->setText(dir);
        m_openDirBtn->setEnabled(true);
        clearError();
    }
}

void ConverterPage::openOutputDir() {
    if (!m_outputDir.isEmpty() && QFileInfo(m_outputDir).isDir()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_outputDir));
    }
}

// 转换先写临时区：确保临时目录存在
void ConverterPage::ensureTempDir() {
    if (m_tempDir.isEmpty() || !QFileInfo(m_tempDir).isDir()) {
        m_tempDir = QDir::temp().filePath(QStringLiteral("AudioLocalQt_%1").arg(QCoreApplication::applicationPid()));
        QDir().mkpath(m_tempDir);
    }
}

// 确保目标目录：未选择则弹出选择框；用户取消返回 false
bool ConverterPage::ensureTargetDir() {
    if (m_outputDir.isEmpty() || !QFileInfo(m_outputDir).isDir()) {
        const QString dir = QFileDialog::getExistingDirectory(
            this, QStringLiteral("选择导出目录"),
            m_outputDir.isEmpty() ? QDir::homePath() : m_outputDir);
        if (dir.isEmpty()) return false;
        m_outputDir = dir;
        m_outputEdit->setText(dir);
        m_openDirBtn->setEnabled(true);
        clearError();
    }
    return true;
}

// 把某个已完成文件从临时区导出到目标目录；未导出过才复制，导出后删除临时区副本
bool ConverterPage::exportFile(FileItem& it) {
    if (it.status != FileItem::Done || it.result.isEmpty()) return false;
    if (it.exported) return true;   // 已导出过，直接视为成功
    if (!ensureTargetDir()) return false;
    const QFileInfo src(it.result);
    if (!src.exists()) return false;
    const QString dest = buildOutputPath(src.absoluteFilePath(), m_outputDir, src.suffix());
    if (!QFile::copy(src.absoluteFilePath(), dest)) {
        setError(s_errorTitle + QStringLiteral(": 导出失败，无法复制文件"));
        return false;
    }
    QFile::remove(src.absoluteFilePath());   // 清掉临时区副本
    it.result = dest;
    it.exported = true;
    return true;
}

void ConverterPage::openItemDir(int row) {
    if (row < 0 || row >= m_items.size()) return;
    const FileItem& it = m_items[row];
    if (it.status != FileItem::Done || !it.exported || it.result.isEmpty()) return;
    // 打开输出文件所在目录（explorer.exe 经 QProcess 常解析失败，改用系统默认文件管理器）
    if (!it.result.isEmpty()) {
        const QFileInfo fi(it.result);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absolutePath()));
    }
}

void ConverterPage::removeItem(int row) {
    if (row < 0 || row >= m_items.size()) return;
    FileItem it = m_items.takeAt(row);
    // 未导出的文件还留在临时区，一并删除
    if (!it.exported && !it.result.isEmpty()) QFile::remove(it.result);
    rebuildFileTable();
}

void ConverterPage::exportItem(int row) {
    if (row < 0 || row >= m_items.size()) return;
    FileItem& it = m_items[row];
    if (it.status != FileItem::Done) return;
    if (exportFile(it)) {
        rebuildFileTable();
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(it.result).absolutePath()));
    }
}

// 导出全部：把所有“已完成未导出”的文件一次导出到目标目录
void ConverterPage::exportAll() {
    if (!ensureTargetDir()) return;
    bool any = false;
    for (FileItem& it : m_items) {
        if (it.status == FileItem::Done && !it.exported) {
            if (exportFile(it)) any = true;
        }
    }
    if (any) {
        rebuildFileTable();
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_outputDir));
    }
}

bool ConverterPage::acceptExt(const QString& ext) const {
    static const QStringList ok = { "mp3", "flac", "wav", "m4a", "aac", "ogg", "opus", "amr", "wma", "ape", "wv" };
    return ok.contains(ext);
}

QStringList ConverterPage::buildArgs(const QString& ext) {
    QStringList args;
    const QString br = m_bitrateCmb->currentData().toString();
    if (br != "auto" && (ext == "mp3" || ext == "m4a" || ext == "ogg" || ext == "opus")) {
        args << "-b:a" << (br + "k");
    }
    const QString sr = m_sampleCmb->currentData().toString();
    if (sr != "auto") args << "-ar" << sr;
    const QString ch = m_channelCmb->currentData().toString();
    if (ch == "mono") args << "-ac" << "1";
    else if (ch == "stereo") args << "-ac" << "2";

    const QMap<QString, QStringList> codecs = {
        { "mp3",  { "-c:a", "libmp3lame" } },
        { "flac", { "-c:a", "flac" } },
        { "wav",  { "-c:a", "pcm_s16le" } },
        { "m4a",  { "-c:a", "aac" } },
        { "ogg",  { "-c:a", "libvorbis" } },
        { "opus", { "-c:a", "libopus" } }
    };
    args << codecs.value(ext);
    return args;
}

QString ConverterPage::buildOutputPath(const QString& input, const QString& dir, const QString& ext) {
    QString name = QFileInfo(input).completeBaseName();
    name.replace(QRegularExpression("[\\\\/:*?\"<>|]"), "_");
    QString base = dir + "/" + name + "." + ext;
    // 若存在则加序号
    QString p = base;
    int n = 1;
    while (QFileInfo::exists(p)) {
        p = dir + "/" + name + " (" + QString::number(n) + ")." + ext;
        ++n;
    }
    return p;
}

void ConverterPage::setError(const QString& msg) {
    m_errorLabel->setText(msg);
    m_errorLabel->show();
}

void ConverterPage::clearError() {
    m_errorLabel->hide();
}

void ConverterPage::startConvert() {
    if (m_worker.isRunning() || m_items.isEmpty()) return;
    ensureTempDir();
    clearError();

    const QString ext = m_formatCmb->currentData().toString();
    const QStringList args = buildArgs(ext);

    // 找到第一个待转换（Idle）项；Error 项不自动重试，避免失败文件被反复重跑
    int idx = -1;
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].status == FileItem::Idle) { idx = i; break; }
    }
    if (idx < 0) return;

    FileItem& it = m_items[idx];
    it.status = FileItem::Converting;
    it.progress = 0;
    it.err.clear();
    it.exported = false;   // 转换结果先落到临时区，导出时才写入目标目录

    // 转换先写临时区，导出时再复制/移动到目标目录
    const QString outPath = buildOutputPath(it.path, m_tempDir, ext);
    m_worker.startConvert(it.path, outPath, args);
    m_currentOut = outPath;

    // 先启动 worker 再重建表格：此时 isRunning()==true，转换按钮才会正确禁用
    m_convertBtn->setText(s_converting);
    m_table->setRowCount(m_items.size());
    rebuildFileTable();
}

void ConverterPage::onProgress(int pct) {
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].status == FileItem::Converting) {
            m_items[i].progress = pct;
            // 只更新转换中那一行的进度条，避免整表重建导致闪烁
            if (auto* bar = qobject_cast<QProgressBar*>(m_table->cellWidget(i, 2))) {
                bar->setValue(pct);
            }
            break;
        }
    }
}

void ConverterPage::onConvertOk(const QString& out) {
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].status == FileItem::Converting) {
            m_items[i].status = FileItem::Done;
            m_items[i].progress = 100;
            m_items[i].result = out;
            break;
        }
    }
    // 继续下一个
    bool hasNext = false;
    for (const FileItem& it : m_items) if (it.status == FileItem::Idle) { hasNext = true; break; }
    if (hasNext) {
        startConvert();
    } else {
        m_convertBtn->setText(s_convert);
        m_convertBtn->setEnabled(true);
        rebuildFileTable();
    }
}

void ConverterPage::onConvertFail(const QString& err) {
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].status == FileItem::Converting) {
            m_items[i].status = FileItem::Error;
            m_items[i].err = err;
            break;
        }
    }
    // 单个文件失败不中断整批，继续转换剩余 Idle 项
    bool hasNext = false;
    for (const FileItem& it : m_items) if (it.status == FileItem::Idle) { hasNext = true; break; }
    if (hasNext) {
        startConvert();
    } else {
        m_convertBtn->setText(s_convert);
        m_convertBtn->setEnabled(true);
        rebuildFileTable();
        setError(s_errorTitle + ": " + err);
    }
}

void ConverterPage::onCancelled() {
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].status == FileItem::Converting) {
            m_items[i].status = FileItem::Idle;
            m_items[i].progress = 0;
            break;
        }
    }
    m_convertBtn->setText(s_convert);
    m_convertBtn->setEnabled(true);
    rebuildFileTable();
    clearError();
}
