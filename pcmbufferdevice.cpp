#include "pcmbufferdevice.h"
#include <QDebug>
PcmBufferDevice::PcmBufferDevice(QObject *parent)
    : QIODevice{parent}
{
    open(QIODevice::ReadOnly);
}

void PcmBufferDevice::append(const QByteArray &data)
{
    if (data.isEmpty())
        return;

    {
        QMutexLocker lock(&m_mutex);
        m_buffer.append(data);
    }

    emit readyRead(); // 🔥 tells QAudioSink/platform backend “data is available”
}


qint64 PcmBufferDevice::bytesAvailable() const
{

    QMutexLocker lock(&m_mutex);
    return m_buffer.size() + QIODevice::bytesAvailable();
    // return m_buffer.size();


}

qint64 PcmBufferDevice::bufferedBytes() const
{
    QMutexLocker lock(&m_mutex);
    return m_buffer.size();
}


qint64 PcmBufferDevice::readData(char *data, qint64 maxlen)
{
    QMutexLocker lock(&m_mutex);

    if(m_buffer.isEmpty())
        return 0;

    qint64 len = qMin<qint64>(maxlen,m_buffer.size());
    memcpy(data,m_buffer.constData(),len);
    m_buffer.remove(0,len);
    return len;

}


