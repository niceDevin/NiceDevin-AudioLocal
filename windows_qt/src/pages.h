#pragma once
#include <QWidget>

class QVBoxLayout;
class QLabel;

// 支持格式页
class FormatsPage : public QWidget {
    Q_OBJECT
public:
    explicit FormatsPage(QWidget* parent = nullptr);
    void retranslate();
    void setLang(bool en);
private:
    QLabel* m_title;
    QLabel* m_sub;
    QLabel* m_inTitle;
    QLabel* m_outTitle;
    QLabel* m_note;
    QVBoxLayout* m_root;
    bool m_en = false;
};

// 使用说明页
class HowPage : public QWidget {
    Q_OBJECT
public:
    explicit HowPage(QWidget* parent = nullptr);
    void retranslate();
    void setLang(bool en);
private:
    QLabel* m_title;
    QLabel* m_step1Title, * m_step1Text;
    QLabel* m_step2Title, * m_step2Text;
    QLabel* m_step3Title, * m_step3Text;
    QLabel* m_privacy;
    QVBoxLayout* m_root;
    bool m_en = false;
};

// 隐私页
class PrivacyPage : public QWidget {
    Q_OBJECT
public:
    explicit PrivacyPage(QWidget* parent = nullptr);
    void retranslate();
    void setLang(bool en);
private:
    QLabel* m_title;
    QLabel* m_p1;
    QLabel* m_p3;
    bool m_en = false;
};
