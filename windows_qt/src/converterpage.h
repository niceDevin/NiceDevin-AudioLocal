#pragma once
#include <QWidget>
#include <QStringList>
#include "ffmpegworker.h"

class QVBoxLayout;
class QLineEdit;
class QComboBox;
class QTableWidget;
class QPushButton;
class QLabel;
class QProgressBar;
class QFrame;

struct FileItem {
    QString path;
    QString name;
    QString result;              // 当前文件位置（转换后为临时区，导出后为目标目录）
    int progress = 0;
    enum Status { Idle, Converting, Done, Error } status = Idle;
    QString err;
    bool exported = false;       // 是否已导出到目标目录
};

// 转换器页：路径输入（浏览/拖拽/手输）+ 输出目录 + 格式选项 + 文件表 + 转码
class ConverterPage : public QWidget {
    Q_OBJECT
public:
    explicit ConverterPage(QWidget* parent = nullptr);
    ~ConverterPage() override;
    void retranslate();
    void setLang(bool en);

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dragLeaveEvent(QDragLeaveEvent* e) override;
    void dropEvent(QDropEvent* e) override;

private slots:
    void pickInputFiles();
    void addFromText();
    void pickOutputDir();
    void startConvert();
    void openOutputDir();
    void openItemDir(int row);
    void removeItem(int row);
    void exportItem(int row);
    void exportAll();
    void onProgress(int pct);
    void onConvertOk(const QString& out);
    void onConvertFail(const QString& err);
    void onCancelled();

private:
    void rebuildFileTable();
    void clearBtnText();
    QStringList buildArgs(const QString& ext);
    QString buildOutputPath(const QString& input, const QString& dir, const QString& ext);
    void setError(const QString& msg);
    void clearError();
    bool acceptExt(const QString& ext) const;
    void ensureTempDir();
    bool ensureTargetDir();
    bool exportFile(FileItem& it);

    FFmpegWorker m_worker;
    QList<FileItem> m_items;
    QString m_outputDir;   // 导出目标目录
    QString m_tempDir;     // 转换临时区
    QString m_currentOut;

    // 控件
    QLabel* m_heroBadge;
    QLabel* m_heroTitle;
    QLabel* m_heroSub;
    QLabel* m_heroPoint;
    QFrame* m_dropZone;
    QLabel* m_dropMain;
    QPushButton* m_browseBtn;
    QLabel* m_errorLabel;
    QLineEdit* m_inputEdit;
    QLineEdit* m_outputEdit;
    QComboBox* m_formatCmb;
    QComboBox* m_bitrateCmb;
    QComboBox* m_sampleCmb;
    QComboBox* m_channelCmb;
    QTableWidget* m_table;
    QPushButton* m_convertBtn;
    QPushButton* m_exportBtn;   // 转换成功后启用：打开导出目录
    QPushButton* m_openDirBtn;
    QLabel* m_panelSub;
    QLabel* m_panelTitle;

    // 文案（中英切换）
    QString s_heroBadge, s_heroTitlePre, s_heroTitleHi, s_heroSub, s_heroPoint,
            s_panelTitle, s_drop, s_browse, s_hint, s_multi, s_placeholder, s_add, s_outLabel,
            s_outPlaceholder, s_openDir, s_convert, s_converting, s_done, s_err,
            s_ready, s_unsupported, s_pathInvalid, s_dropFailed, s_outRequired,
            s_clear, s_countNote, s_formatLabel, s_bitrateLabel, s_sampleLabel, s_chanLabel,
            s_auto, s_stereo, s_mono, s_openOut, s_remove, s_errorTitle, s_cancel, s_export,
            s_exported;
    QStringList s_fmtNames; // 格式显示名
    bool m_dragging = false;
    bool m_en = false;
};
