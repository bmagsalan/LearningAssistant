#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class NetworkModel:public QObject{
    Q_OBJECT
    Q_PROPERTY(QString buffer READ buffer NOTIFY bufferChanged)
private:
    QNetworkAccessManager* m_network = nullptr;
    QNetworkReply* m_reply = nullptr;
    QString m_buffer;
public:
    explicit NetworkModel(QObject* parent = nullptr);

    void sendRequest(QUrl htmlAddress);



    QString buffer() const;
signals:
    void bufferChanged(const QString& htmlString);
};

#endif // NETWORKMODEL_H
