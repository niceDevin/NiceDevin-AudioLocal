#include "mainwindow.h"
#include "converterpage.h"
#include "pages.h"
#include "style.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QScrollArea>
#include <QFrame>
#include <QDate>
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setObjectName("Root");
    setWindowTitle(QStringLiteral("NiceDevin AudioLocal · 本地音频转换器"));
    resize(1100, 780);
    setMinimumSize(920, 640);

    auto* root = new QWidget(this);
    root->setObjectName("Root");
    auto* rootLay = new QVBoxLayout(root);
    rootLay->setContentsMargins(0, 0, 0, 0);
    rootLay->setSpacing(0);

    // 顶栏
    auto* topBar = new QWidget(root);
    topBar->setObjectName("TopBar");
    auto* topLay = new QHBoxLayout(topBar);
    topLay->setContentsMargins(28, 0, 28, 0);
    topLay->setSpacing(16);

    auto* brandMark = new QLabel(topBar);
    brandMark->setObjectName("BrandMark");
    brandMark->setFixedSize(20, 20);
    // 顶部图标与窗口图标一致（同一张资源）
    const QPixmap brandPm(QStringLiteral(":/assets/audiolocal-icon.png"));
    brandMark->setPixmap(brandPm.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    brandMark->setAlignment(Qt::AlignCenter);
    auto* brand = new QLabel(topBar);
    brand->setObjectName("Brand");
    brand->setText(QStringLiteral("NiceDevin AudioLocal"));

    topLay->addWidget(brandMark);
    topLay->addWidget(brand);
    topLay->addSpacing(24);

    // 导航
    m_navBtns[0] = new QPushButton(topBar);
    m_navBtns[1] = new QPushButton(topBar);
    m_navBtns[2] = new QPushButton(topBar);
    m_navBtns[3] = new QPushButton(topBar);
    for (int i = 0; i < 4; ++i) {
        m_navBtns[i]->setObjectName("NavBtn");
        m_navBtns[i]->setCheckable(true);
        m_navBtns[i]->setCursor(Qt::PointingHandCursor);
        m_navBtns[i]->setMinimumHeight(34);
        m_navBtns[i]->setMinimumWidth(120);
        connect(m_navBtns[i], &QPushButton::clicked, this, [this, i]() { switchPage(i); });
        topLay->addWidget(m_navBtns[i]);
    }
    topLay->addStretch();

    m_langBtn = new QPushButton(topBar);
    m_langBtn->setObjectName("LangBtn");
    m_langBtn->setCursor(Qt::PointingHandCursor);
    m_langBtn->setMinimumHeight(34);
    m_langBtn->setMinimumWidth(88);
    connect(m_langBtn, &QPushButton::clicked, this, &MainWindow::toggleLang);
    topLay->addWidget(m_langBtn);

    rootLay->addWidget(topBar);

    // 页面堆叠（每页包进滚动区，内容超出时可滚动，不会挤压裁切文字）
    m_stack = new QStackedWidget(root);
    m_stack->setObjectName("Pages");
    m_converter = new ConverterPage(m_stack);
    m_formats = new FormatsPage(m_stack);
    m_how = new HowPage(m_stack);
    m_privacy = new PrivacyPage(m_stack);

    auto wrapInScroll = [this](QWidget* page) -> QScrollArea* {
        auto* area = new QScrollArea(m_stack);
        area->setObjectName("PageScroll");
        area->setWidgetResizable(true);
        area->setFrameShape(QFrame::NoFrame);
        area->setWidget(page);
        return area;
    };
    m_stack->addWidget(wrapInScroll(m_converter));
    m_stack->addWidget(wrapInScroll(m_formats));
    m_stack->addWidget(wrapInScroll(m_how));
    m_stack->addWidget(wrapInScroll(m_privacy));
    rootLay->addWidget(m_stack, 1);

    // 底栏
    auto* footer = new QWidget(root);
    footer->setObjectName("Footer");
    auto* footLay = new QHBoxLayout(footer);
    footLay->setContentsMargins(28, 16, 28, 16);
    footLay->setSpacing(20);

    auto* footBrand = new QLabel(footer);
    footBrand->setObjectName("FooterBrand");
    footBrand->setText(QStringLiteral("NiceDevin AudioLocal"));
    m_footerTag = new QLabel(footer);
    m_footerTag->setObjectName("FooterTag");
    m_footerCopy = new QLabel(footer);
    m_footerCopy->setObjectName("FooterCopy");

    footLay->addWidget(footBrand);
    footLay->addWidget(m_footerTag);
    footLay->addStretch();
    footLay->addWidget(m_footerCopy);

    rootLay->addWidget(footer);
    setCentralWidget(root);

    // 初始状态
    m_navBtns[0]->setChecked(true);
    retranslate();
}

void MainWindow::switchPage(int index) {
    m_stack->setCurrentIndex(index);
    for (int i = 0; i < 4; ++i) m_navBtns[i]->setChecked(i == index);
}

void MainWindow::toggleLang() {
    m_en = !m_en;
    retranslate();
    m_converter->setLang(m_en);
    m_formats->setLang(m_en);
    m_how->setLang(m_en);
    m_privacy->setLang(m_en);
}

void MainWindow::retranslate() {
    if (m_en) {
        setWindowTitle(QStringLiteral("NiceDevin AudioLocal · Local Audio Converter"));
        m_navBtns[0]->setText(QStringLiteral("Converter"));
        m_navBtns[1]->setText(QStringLiteral("Formats"));
        m_navBtns[2]->setText(QStringLiteral("How it works"));
        m_navBtns[3]->setText(QStringLiteral("Privacy"));
        m_langBtn->setText(QStringLiteral("中文"));
        m_footerTag->setText(QStringLiteral("Local conversion · No server"));
        m_footerCopy->setText(QStringLiteral("© %1 NiceDevin · All rights reserved").arg(QString::number(QDate::currentDate().year())));
    } else {
        setWindowTitle(QStringLiteral("NiceDevin AudioLocal · 本地音频转换器"));
        m_navBtns[0]->setText(QStringLiteral("转换器"));
        m_navBtns[1]->setText(QStringLiteral("支持格式"));
        m_navBtns[2]->setText(QStringLiteral("使用说明"));
        m_navBtns[3]->setText(QStringLiteral("隐私"));
        m_langBtn->setText(QStringLiteral("English"));
        m_footerTag->setText(QStringLiteral("本地转码 · 无服务器"));
        m_footerCopy->setText(QStringLiteral("© %1 NiceDevin · 保留所有权利").arg(QString::number(QDate::currentDate().year())));
    }
}
