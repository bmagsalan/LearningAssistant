#include "ttsviewmodel.h"

#include "piperttsservice.h"

TtsViewModel::TtsViewModel(QObject *parent)
    : QObject{parent}
{
    m_tts = new PiperTtsService(this);

    connect(m_tts,&PiperTtsService::speakingStarted,this,[this](){
        // if(!m_speaking)
        // {
        //     m_speaking = true;
        //     emit speakingChanged(m_speaking);
        // }

    });
    connect(m_tts,&PiperTtsService::speakingFinished,this,[this](){
        if(m_speaking)
        {
            m_speaking = false;
            emit speakingChanged(m_speaking);
        }
    });
    connect(m_tts, &PiperTtsService::errorOccurred, this, [this](const QString& msg) {
        emit errorOccurred(msg);
    });

}


void TtsViewModel::speak(const QString& text)
{
    if(m_speaking)
        return;

    m_speaking = true;
    emit speakingChanged(m_speaking);

    if (text.trimmed().isEmpty())
        return;

    m_tts->speak(text);
}

void TtsViewModel::stop()
{
    m_tts->stop();
}

bool TtsViewModel::speaking() const
{
    return m_speaking;
}
