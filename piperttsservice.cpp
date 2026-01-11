#include "piperttsservice.h"
#include "pcmbufferdevice.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

static const int BUFFER_SIZE = 128 * 1024;
static bool startedEmitted = false;

PiperTtsService::PiperTtsService(QObject *parent)
    : QObject{parent}
{}

// -------------------------------------------------
// Audio setup
// -------------------------------------------------
void PiperTtsService::setupAudio()
{
    if (m_audioSink)
        return;

    QAudioFormat format;
    format.setSampleRate(22050);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice dev = QMediaDevices::defaultAudioOutput();
    if (!dev.isFormatSupported(format)) {
        qWarning() << "Format not supported, using preferred";
        format = dev.preferredFormat();
    }

    m_pcmDevice = new PcmBufferDevice(this);

    m_audioSink = new QAudioSink(dev, format, this);
    m_audioSink->setBufferSize(BUFFER_SIZE);

    connect(m_audioSink, &QAudioSink::stateChanged, this,
            [this](QAudio::State state) {

                if (state == QAudio::IdleState && startedEmitted) {
                    startedEmitted = false;
                    emit speakingFinished();   // 🔥 playback truly finished
                }
                if (state == QAudio::ActiveState && !startedEmitted) {
                    startedEmitted = true;
                    emit speakingStarted();   // 🔥 speaking truly begins here
                }

            });


    m_audioSink->start(m_pcmDevice);
}

// -------------------------------------------------
// Start Piper if needed
// -------------------------------------------------
void PiperTtsService::startProcessIfNeeded()
{
    if (m_proc && m_proc->state() != QProcess::NotRunning)
        return;


    setupAudio();

    QString baseDir  = QCoreApplication::applicationDirPath();
    QString piperDir = QDir(baseDir).filePath("piper");

#ifdef Q_OS_WIN
    QString piperExe = QDir(piperDir).filePath("piper.exe");
#else
    QString piperExe = QDir(piperDir).filePath("piper");
#endif

    QString model  = QDir(piperDir).filePath("en_US-amy-medium.onnx");
    QString config = QDir(piperDir).filePath("en_US-amy-medium.onnx.json");
    QString espeak = QDir(piperDir).filePath("espeak-ng-data");

    // ---------- SANITY ASSERTS (DEBUG ONLY) ----------
    Q_ASSERT_X(QFileInfo::exists(piperExe),
               "PiperTtsService",
               qPrintable(QString("Missing piper executable: %1").arg(piperExe)));

    Q_ASSERT_X(QFileInfo::exists(model),
               "PiperTtsService",
               qPrintable(QString("Missing model file: %1").arg(model)));

    Q_ASSERT_X(QFileInfo::exists(config),
               "PiperTtsService",
               qPrintable(QString("Missing config file: %1").arg(config)));

    Q_ASSERT_X(QFileInfo(espeak).exists() && QFileInfo(espeak).isDir(),
               "PiperTtsService",
               qPrintable(QString("Missing espeak-ng-data directory: %1").arg(espeak)));
    // -----------------------------------------------

    m_proc = new QProcess(this);
    m_proc->setWorkingDirectory(piperDir);

    QStringList args;
    args << "-m" << model
         << "-c" << config
         << "--espeak_data" << espeak
         << "--output_raw";

    connect(m_proc, &QProcess::readyReadStandardOutput, this, [this]() {
        QByteArray pcm = m_proc->readAllStandardOutput();
        if (pcm.isEmpty())
        {
            return;

        }





        m_pcmDevice->append(pcm);
    });

    connect(m_proc, &QProcess::readyReadStandardError, this, [this]() {
        qDebug() << "Piper:" << m_proc->readAllStandardError();
    });

    connect(m_proc, &QProcess::finished, this, [this](int, QProcess::ExitStatus) {

        cleanup();
    });

    m_proc->start(piperExe, args);

    if (!m_proc->waitForStarted(3000)) {
        emit errorOccurred("Failed to start Piper process");
        cleanup();
    }
}

// -------------------------------------------------
// Public API
// -------------------------------------------------
void PiperTtsService::speak(const QString &text)
{
    startProcessIfNeeded();

    if (!m_proc || m_proc->state() != QProcess::Running)
        return;

    m_proc->write(text.toUtf8());
    m_proc->write("\n");

}

void PiperTtsService::stop()
{
    cleanup();
}

bool PiperTtsService::isSpeaking() const
{
    return m_proc && m_proc->state() == QProcess::Running;
}

// -------------------------------------------------
// Cleanup
// -------------------------------------------------
void PiperTtsService::cleanup()
{
    if (m_proc) {
        m_proc->kill();
        m_proc->deleteLater();
        m_proc = nullptr;
    }

    if (m_audioSink) {
        m_audioSink->stop();
        m_audioSink->deleteLater();
        m_audioSink = nullptr;
    }

    if (m_pcmDevice) {
        m_pcmDevice->deleteLater();
        m_pcmDevice = nullptr;
    }
}
