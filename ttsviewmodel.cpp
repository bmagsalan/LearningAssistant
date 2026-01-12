#include "ttsviewmodel.h"

#include "piperttsservice.h"
#include "networkmodel.h"
#include "articleextractor.h"

#include <QApplication>
#include <QRegularExpression>

TtsViewModel::TtsViewModel(QObject *parent)
    : QObject{parent},
    extractor(new ArticleExtractor(this))
{
    m_net = new NetworkModel(this);
    m_tts = new PiperTtsService(this);

    connect(m_net,&NetworkModel::bufferChanged,this,[this](const QString& str){
        emit htmlReceived(str);
    });

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

    connect(extractor, &ArticleExtractor::finished, this,
            [this](const QString& html) {
                QString cleanHtml = html;
                cleanHtml.remove(QRegularExpression("<img[^>]*>"));
                emit htmlReceived(cleanHtml);
            });

    connect(extractor, &ArticleExtractor::failed, this,
            [this](const QString& err) {
                emit htmlReceived("<b>Error:</b> " + err);
                // m_txt1->setHtml("<b>Error:</b> " + err);
            });


}

void TtsViewModel::sendHtmlRequest(QUrl htmAddress)
{
    // if( m_net )
    // {
    //     m_net->sendRequest(htmAddress);
    // }

    if( extractor )
        extractor->extract(htmAddress);

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
