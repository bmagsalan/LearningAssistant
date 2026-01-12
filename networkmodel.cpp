#include "networkmodel.h"

NetworkModel::NetworkModel(QObject *parent)
    : QObject{parent}
{

    m_network = new QNetworkAccessManager(this);

}

void NetworkModel::sendRequest(QUrl htmlAddress)
{
    QNetworkRequest req(htmlAddress);
    m_reply = m_network->get(req);

    connect(m_reply,&QNetworkReply::readyRead,this,[this](){
        m_buffer.append(m_reply->readAll());
    });

    connect(m_reply,&QNetworkReply::downloadProgress,this,[this](qint64 bytesReceived, qint64 bytesTotal){
        qDebug() << QString("%1/%2").arg(QString::number(bytesReceived)).arg(QString::number(bytesTotal));
    });

    connect(m_reply,&QNetworkReply::finished,this,[this](){
        qDebug() << m_buffer;
        emit bufferChanged(m_buffer);
        m_reply->deleteLater();
        m_reply = nullptr;

    });
}
QString NetworkModel::buffer() const
{
    return m_buffer;
}
