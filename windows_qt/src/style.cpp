#include "style.h"

QString Style::appStyleSheet() {
    // 复刻网页版视觉：暖白纸底 + 墨黑 + 铜橙强调，发丝边框，克制圆角
    return QString(R"QSS(
* {
    font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
    font-size: 14px;
    color: #1c1a16;
}

QMainWindow, QWidget#Root {
    background-color: #f4f1ea;
}

/* ===== 顶栏 ===== */
QWidget#TopBar {
    background-color: #f4f1ea;
    border-bottom: 1px solid #ddd6c6;
}
QLabel#Brand {
    font-size: 17px;
    font-weight: 700;
    color: #1c1a16;
}
QPushButton#NavBtn {
    background: transparent;
    border: none;
    border-bottom: 2px solid transparent;
    color: #55503f;
    font-size: 14px;
    min-height: 20px;
    padding: 6px 12px;
}
QPushButton#NavBtn:hover {
    color: #c2571a;
    border-bottom-color: #c2571a;
}
QPushButton#NavBtn:checked {
    color: #c2571a;
    border-bottom-color: #c2571a;
    font-weight: 600;
}
QPushButton#LangBtn {
    background-color: #fbfaf6;
    border: 1px solid #c9c1ac;
    border-radius: 6px;
    color: #1c1a16;
    font-size: 13px;
    min-height: 20px;
    padding: 6px 14px;
}
QPushButton#LangBtn:hover {
    border-color: #c2571a;
    color: #9a430f;
}

/* ===== 内容 ===== */
QStackedWidget#Pages {
    background-color: #f4f1ea;
}
QScrollArea {
    background: transparent;
    border: none;
}

/* ===== Hero ===== */
QLabel#HeroBadge {
    background-color: #fbfaf6;
    border: 1px solid #c9c1ac;
    border-radius: 999px;
    color: #55503f;
    font-size: 12px;
    padding: 6px 16px;
}
QLabel#HeroTitle {
    font-size: 40px;
    font-weight: 700;
    color: #1c1a16;
}
QLabel#HeroSub {
    color: #55503f;
    font-size: 16px;
}
QLabel#HeroPoint {
    color: #8b8570;
    font-size: 13px;
}
QPushButton#BrowseLink {
    background: transparent;
    border: none;
    color: #c2571a;
    font-size: 16px;
    font-weight: 600;
    min-height: 20px;
    padding: 2px 4px;
}
QPushButton#BrowseLink:hover {
    color: #9a430f;
    text-decoration: underline;
}

/* ===== 主按钮 ===== */
QPushButton#BtnPrimary {
    background-color: #c2571a;
    border: 1px solid #c2571a;
    border-radius: 6px;
    color: #17150f;
    font-size: 15px;
    font-weight: 600;
    min-height: 24px;
    padding: 12px 24px;
}
QPushButton#BtnPrimary:hover {
    background-color: #9a430f;
}
QPushButton#BtnPrimary:disabled {
    background-color: #e0b89a;
    border-color: #e0b89a;
    color: #7a7468;
}
QPushButton#BtnSecondary {
    background-color: #fbfaf6;
    border: 1px solid #c9c1ac;
    border-radius: 6px;
    color: #1c1a16;
    font-size: 14px;
    font-weight: 600;
    min-height: 22px;
    padding: 10px 20px;
}
QPushButton#BtnSecondary:hover {
    border-color: #c2571a;
}
QPushButton#BtnSecondary:disabled {
    color: #b9b3a4;
    border-color: #ddd6c6;
}
QPushButton#BtnGhost {
    background: transparent;
    border: none;
    color: #8b8570;
    font-size: 13px;
    min-height: 20px;
    padding: 8px 12px;
}
QPushButton#BtnGhost:hover { color: #b03a2e; }
QPushButton#TableBtn {
    background-color: #f4f1ea;
    border: 1px solid #c9c1ac;
    border-radius: 6px;
    color: #1c1a16;
    font-size: 12px;
    min-height: 20px;
    padding: 3px 12px;
}
QPushButton#TableBtn:hover { border-color: #c2571a; color: #9a430f; }
QPushButton#TableBtn:disabled { color: #b9b3a4; border-color: #ddd6c6; }

/* ===== 卡片 / 面板 ===== */
QFrame#Panel {
    background-color: #fbfaf6;
    border: 1px solid #ddd6c6;
    border-radius: 10px;
}
QLabel#PanelTitle {
    font-size: 22px;
    font-weight: 700;
}
QLabel#PanelSub {
    color: #8b8570;
    font-size: 13px;
}

/* ===== 拖放区 ===== */
QFrame#DropZone {
    background-color: #fbfaf6;
    border: 1.5px dashed #c9c1ac;
    border-radius: 10px;
}
QFrame#DropZone[dragActive="true"] {
    border-color: #c2571a;
    background-color: #f6e3d2;
}
QLabel#DropMain { font-size: 16px; }
QLabel#DropHint { color: #8b8570; font-size: 13px; }

/* ===== 输入框 ===== */
QLineEdit#PathInput, QLineEdit#OutputInput {
    background-color: #f4f1ea;
    border: 1px solid #c9c1ac;
    border-radius: 6px;
    padding: 9px 12px;
    font-size: 13px;
    font-family: "Consolas", "Courier New", monospace;
}
QLineEdit#PathInput:focus, QLineEdit#OutputInput:focus {
    border-color: #c2571a;
}
QComboBox#CmbBox {
    background-color: #f4f1ea;
    border: 1px solid #c9c1ac;
    border-radius: 6px;
    padding: 8px 12px;
    font-size: 13px;
}
QComboBox#CmbBox:hover { border-color: #c2571a; }
QComboBox#CmbBox::drop-down { border: none; width: 22px; }

/* ===== 格式选项按钮 ===== */
QPushButton#FmtBtn {
    background-color: #f4f1ea;
    border: 1px solid #c9c1ac;
    border-radius: 6px;
    text-align: left;
    padding: 10px 14px;
}
QPushButton#FmtBtn:checked {
    background-color: #f6e3d2;
    border-color: #c2571a;
}
QLabel#FmtExt {
    font-size: 18px;
    font-weight: 700;
}
QLabel#FmtDesc {
    color: #55503f;
    font-size: 12px;
}

/* ===== 文件列表 ===== */
QTableWidget#FileTable {
    background-color: #fbfaf6;
    border: 1px solid #ddd6c6;
    border-radius: 6px;
    gridline-color: #ddd6c6;
    selection-background-color: #f6e3d2;
    selection-color: #1c1a16;
}
QTableWidget#FileTable::item { padding: 6px 8px; }
QHeaderView::section {
    background-color: #f4f1ea;
    border: none;
    border-bottom: 1px solid #ddd6c6;
    padding: 8px;
    font-weight: 600;
}

/* ===== 进度条 ===== */
QProgressBar#PBar {
    background-color: #ddd6c6;
    border: none;
    border-radius: 6px;
    height: 14px;
    text-align: center;
    color: #1c1a16;
    font-size: 11px;
    min-width: 96px;
}
QProgressBar#PBar::chunk {
    background-color: #c2571a;
    border-radius: 3px;
}

/* ===== 标签 ===== */
QLabel#TagOk {
    background-color: #e6efe6;
    color: #3a7d44;
    border-radius: 6px;
    padding: 3px 10px;
    font-size: 12px;
}
QLabel#TagErr {
    background-color: #f4e2df;
    color: #b03a2e;
    border-radius: 6px;
    padding: 3px 10px;
    font-size: 12px;
}
QLabel#TagIdle {
    background-color: #f6e3d2;
    color: #9a430f;
    border-radius: 6px;
    padding: 3px 10px;
    font-size: 12px;
}
QLabel#ErrorNote {
    color: #b03a2e;
    font-size: 13px;
}

/* ===== 章节标题 ===== */
QLabel#SecTitle {
    font-size: 26px;
    font-weight: 700;
}
QLabel#SecSub {
    color: #55503f;
    font-size: 14px;
}
QLabel#StepNum {
    color: #c2571a;
    font-size: 15px;
    font-weight: 700;
}

/* ===== 底栏 ===== */
QWidget#Footer {
    background-color: #fbfaf6;
    border-top: 1px solid #ddd6c6;
}
QLabel#FooterBrand { font-weight: 700; }
QLabel#FooterTag, QLabel#FooterCopy { color: #8b8570; font-size: 13px; }
)QSS");
}
