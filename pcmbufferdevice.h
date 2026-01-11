#ifndef PCMBUFFERDEVICE_H
#define PCMBUFFERDEVICE_H

#include <QIODevice>
#include <QObject>
#include <QMutex>

class PcmBufferDevice : public QIODevice
{
    Q_OBJECT
public:
    explicit PcmBufferDevice(QObject *parent = nullptr);

    void append(const QByteArray& data);
    bool isSequential() const override { return true; }
    qint64 bytesAvailable() const override;
    qint64 bufferedBytes() const;


protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char*, qint64) override{return 0;}
private:
    QByteArray m_buffer;
    mutable QMutex m_mutex;
};

#endif // PCMBUFFERDEVICE_H
