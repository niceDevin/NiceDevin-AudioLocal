#pragma once
#include <QMainWindow>
#include <QString>

class QStackedWidget;
class QPushButton;
class QLabel;
class ConverterPage;
class FormatsPage;
class HowPage;
class PrivacyPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void switchPage(int index);
    void toggleLang();

private:
    void retranslate();
    void buildHeader();
    void buildFooter();

    ConverterPage* m_converter;
    FormatsPage* m_formats;
    HowPage* m_how;
    PrivacyPage* m_privacy;
    QStackedWidget* m_stack;
    QPushButton* m_navBtns[4];
    QPushButton* m_langBtn;
    QLabel* m_footerTag;
    QLabel* m_footerCopy;
    bool m_en = false; // false=中文, true=English
};
