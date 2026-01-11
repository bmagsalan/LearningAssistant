#ifndef PIPERTTSSERVICE_H
#define PIPERTTSSERVICE_H

#include <QObject>
#include <QProcess>
#include <QAudioSink>
#include <QAudioFormat>
#include <QMediaDevices>

class PcmBufferDevice;
class PiperTtsService : public QObject
{
    Q_OBJECT
public:
    explicit PiperTtsService(QObject *parent = nullptr);
    void speak(const QString& text);
    void stop();
    bool isSpeaking() const;

signals:
    void speakingStarted();
    void speakingFinished();
    void errorOccurred(const QString& msg);
private:
    void startProcessIfNeeded();
    void setupAudio();
    void cleanup();

private:
    QProcess* m_proc = nullptr;
    QAudioSink* m_audioSink = nullptr;
    PcmBufferDevice* m_pcmDevice = nullptr;

};

#endif // PIPERTTSSERVICE_H
