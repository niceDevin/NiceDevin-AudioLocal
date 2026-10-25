#include "ffmpegworker.h"
#include <QFileInfo>
#include <QRegularExpression>

FFmpegWorker::FFmpegWorker(QObject* parent)
    : QObject(parent)
{
    connect(&m_proc, &QProcess::readyReadStandardError, this, [this]() {
        const QString all = QString::fromUtf8(m_proc.readAllStandardError());
        const QStringList lines = all.split('\n', Qt::SkipEmptyParts);
        for (const QString& l : lines) parseStderrLine(l.trimmed());
    });
    connect(&m_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus) {
        m_running = false;
        if (m_cancelled) {
            m_cancelled = false;
            emit cancelled();
            return; // 用户取消，不发 finishedOk
        }
        if (exitCode == 0 && QFileInfo::exists(m_outputPath)) {
            emit finishedOk(m_outputPath);
        } else {
            emit failed(QStringLiteral("ffmpeg 退出码 %1").arg(exitCode));
        }
    });
    connect(&m_proc, &QProcess::errorOccurred, this, [this](QProcess::ProcessError err) {
        m_running = false;
        if (err == QProcess::FailedToStart) {
            emit failed(QStringLiteral("无法启动 ffmpeg: %1").arg(m_ffmpegPath));
        }
    });
}

void FFmpegWorker::startConvert(const QString& inputPath, const QString& outputPath, const QStringList& args)
{
    if (m_running) return;

    m_outputPath = outputPath;
    m_durationSec = 0;
    m_cancelled = false;

    QStringList all;
    all << QStringLiteral("-y") << QStringLiteral("-i") << inputPath << args << outputPath;

    m_proc.start(m_ffmpegPath, all);
    m_running = m_proc.waitForStarted(5000);
}

void FFmpegWorker::cancel()
{
    if (m_running) {
        m_cancelled = true;
        m_proc.kill();
    }
}

void FFmpegWorker::parseStderrLine(const QString& line)
{
    static const QRegularExpression durRe(QStringLiteral("Duration:\\s*(\\d+):(\\d+):(\\d+\\.?\\d*)"));
    static const QRegularExpression timeRe(QStringLiteral("time=(\\d+):(\\d+):(\\d+\\.?\\d*)"));

    QRegularExpressionMatch durM = durRe.match(line);
    if (durM.hasMatch() && m_durationSec <= 0) {
        m_durationSec = durM.captured(1).toDouble() * 3600
                      + durM.captured(2).toDouble() * 60
                      + durM.captured(3).toDouble();
    }
    QRegularExpressionMatch timeM = timeRe.match(line);
    if (timeM.hasMatch() && m_durationSec > 0) {
        const double t = timeM.captured(1).toDouble() * 3600
                       + timeM.captured(2).toDouble() * 60
                       + timeM.captured(3).toDouble();
        int pct = int(t / m_durationSec * 100);
        pct = qBound(0, pct, 99);
        emit progressChanged(pct);
    }
}
