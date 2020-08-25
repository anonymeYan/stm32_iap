#ifndef YMODEMSEND_H
#define YMODEMSEND_H

#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QObject>
#include <QSerialPort>
#include "Ymodem.h"

#define READ_TIME_OUT   (200)
#define WRITE_TIME_OUT  (100)

class YmodemSend :  public Ymodem
{
 Q_OBJECT
public:
    explicit YmodemSend();

    void setFileName(const QString &name);

    void setPortName(const QString &name);
    void setPortBaudRate(qint32 baudrate);

    bool startTransmit();
    void stopTransmit();

    int getTransmitProgress();
    Status getTransmitStatus();

signals:
    void transmitProgress(int progress);
    void transmitStatus(YmodemSend::Status status);

//    void transmitDebugMessge(const QString& Messge);


private slots:
    void readTimeOut();
    void writeTimeOut();
    //    void SerialRecvMsgEvent();


private:
    Code callback(Status status, uint8_t *buff, uint32_t *len);

    uint32_t read(uint8_t *buff, uint32_t len);
    uint32_t write(uint8_t *buff, uint32_t len);

    QFile       *file;
    QTimer      *readTimer;
    QTimer      *writeTimer;
    QSerialPort *serialPort;

    int      progress;
    Status   status;
    uint64_t fileSize;
    uint64_t fileCount;

signals:

public slots:
};

#endif // YMODEMSEND_H
