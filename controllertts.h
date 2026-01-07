#ifndef CONTROLLERTTS_H
#define CONTROLLERTTS_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <piper.h>
#include <alsa/asoundlib.h>

class ControllerTts : public QObject
{
    Q_OBJECT
public:
    static ControllerTts* instance(QObject *parent = nullptr);

    void release();
    void speak(const std::string &textToSpeak);
public slots:
    void init();
private:
    explicit ControllerTts(QObject *parent = nullptr);
    piper_synthesizer *synth;

    bool playRawFile(const std::string &filename);

    Q_DISABLE_COPY(ControllerTts)
signals:
};

#endif // CONTROLLERTTS_H
