#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>

// ffmpeg 转码封装：QProcess 调用 ffmpeg.exe，解析进度与错误
class FFmpegWorker : public QObject {
    Q_OBJECT
public:
    explicit FFmpegWorker(QObject* parent = nullptr);

    // 设置 ffmpeg.exe 路径（开发期：bin/ffmpeg.exe）
    void setFfmpegPath(const QString& p) { m_ffmpegPath = p; }
    QString ffmpegPath() const { return m_ffmpegPath; }

    // 启动转码：异步，通过信号回报
    void startConvert(const QString& inputPath, const QString& outputPath, const QStringList& args);

    // 终止当前任务
    void cancel();

    bool isRunning() const { return m_running; }

signals:
    void progressChanged(int percent);
    void finishedOk(const QString& outputPath);
    void failed(const QString& errorMessage);
    void cancelled();

private:
    QProcess m_proc;
    QString m_ffmpegPath;
    QString m_outputPath;
    bool m_running = false;
    bool m_cancelled = false;
    double m_durationSec = 0;

    void parseStderrLine(const QString& line);
};
