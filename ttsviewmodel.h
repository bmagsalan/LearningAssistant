#ifndef TTSVIEWMODEL_H
#define TTSVIEWMODEL_H

#include <QObject>


class PiperTtsService;
class NetworkModel;
class ArticleExtractor;
class TtsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool speaking READ speaking NOTIFY speakingChanged)
public:
    explicit TtsViewModel(QObject *parent = nullptr);

public:
    Q_INVOKABLE void speak(const QString& text);
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool speaking() const;

    Q_INVOKABLE void sendHtmlRequest(QUrl htmAddress);
signals:
    void speakingChanged(bool);
    void htmlReceived(const QString& msg);
    void errorOccurred(const QString& msg);
private:
    bool m_speaking = false;
    PiperTtsService* m_tts = nullptr;
    NetworkModel* m_net = nullptr;
    ArticleExtractor* extractor = nullptr;
};

#endif // TTSVIEWMODEL_H

