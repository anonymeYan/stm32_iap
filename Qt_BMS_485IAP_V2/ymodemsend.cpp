#include "ymodemsend.h"
#include "mainwindow.h"

YmodemSend::YmodemSend()
{
    file = new QFile;
    readTimer=new QTimer;
    writeTimer=new QTimer;
    serialPort=new QSerialPort;

    serialPort->setPortName("COM1");
    serialPort->setBaudRate(115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(readTimer, SIGNAL(timeout()), this, SLOT(readTimeOut()));
    connect(writeTimer, SIGNAL(timeout()), this, SLOT(writeTimeOut()));

}

void YmodemSend::setFileName(const QString &name)
{
    file->setFileName(name);
}

void YmodemSend::setPortName(const QString &name)
{
    serialPort->setPortName(name);
}

void YmodemSend::setPortBaudRate(qint32 baudrate)
{
    serialPort->setBaudRate(baudrate);
}




bool YmodemSend::startTransmit()
{
    progress = 0;
    transmitProgress(progress);
    status   = StatusEstablish;

    if(serialPort->open(QSerialPort::ReadWrite) == true)
    {
        qDebug()<<QStringLiteral("传输串口打开");
        transmitDebugMessge(QStringLiteral("Qt传输串口打开\n"));
        readTimer->start(READ_TIME_OUT);
        transmitDebugMessge(QStringLiteral("Qt打开读定时器\n"));
        return true;
    }
    else
    {
        qDebug()<<QStringLiteral("传输串口打开失败");
        transmitDebugMessge(QStringLiteral("Qt传输串口打开失败\n"));
        return false;
    }
}



//void YmodemSend::SerialRecvMsgEvent()
//{
//    if(serialPort->bytesAvailable() >= 0)
//    {
//        QByteArray buf;
//        QString np;
//        buf = serialPort->readAll();
//        np  = QString(buf);
//        if(!buf.isEmpty())
//        {
//            ymodemsendSerialPotrSignal(buf);
//        }
//    }
//    else
//    {
//        qDebug()<<"接受数据出错" + QString::number(serialPort->bytesAvailable());
//    }

//}










void YmodemSend::stopTransmit()
{
    qDebug()<<QStringLiteral("停止传输");
    transmitDebugMessge(QStringLiteral("Qt停止传输\n"));
    file->close();
    abort();
    status = StatusAbort;
    writeTimer->start(WRITE_TIME_OUT);
    qDebug()<<QStringLiteral("stopTransmit ");
}

int YmodemSend::getTransmitProgress()
{
    return progress;
}

Ymodem::Status YmodemSend::getTransmitStatus()
{
    return status;
}

void YmodemSend::readTimeOut()
{
    readTimer->stop();

    transmit();

    if((status == StatusEstablish) || (status == StatusTransmit))
    {
        readTimer->start(READ_TIME_OUT);
    }
}

void YmodemSend::writeTimeOut()
{
    writeTimer->stop();
    serialPort->close();
    transmitDebugMessge(QStringLiteral("关闭文件发送串口\n"));
    qDebug()<<QStringLiteral("写定时器到时");
    transmitDebugMessge(QStringLiteral("写定时器到时\n"));
    transmitStatus(status);
}


Ymodem::Code YmodemSend::callback(Status status, uint8_t *buff, uint32_t *len)
{
    switch(status)
    {
    case StatusEstablish:
    {
        if(file->open(QFile::ReadOnly) == true)
        {
            QFileInfo fileInfo(*file);

            fileSize  = fileInfo.size();
            fileCount = 0;

            strcpy((char *)buff, fileInfo.fileName().toLocal8Bit().data());//文件名称  把ASCII码转成十六进制
            strcpy((char *)buff + fileInfo.fileName().toLocal8Bit().size() + 1, QByteArray::number(fileInfo.size()).data());//文件大小

            *len = YMODEM_PACKET_SIZE;

            YmodemSend::status = StatusEstablish;

            transmitStatus(StatusEstablish);

            return CodeAck;
        }
        else
        {
            YmodemSend::status = StatusError;

            qDebug()<<QStringLiteral("callback StatusEstablish 打开文件失败");
            transmitDebugMessge(QStringLiteral("打开文件失败\n"));
            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }
    }

    case StatusTransmit:
    {
        if(fileSize != fileCount)
        {
            if((fileSize - fileCount) > YMODEM_PACKET_SIZE)
            {
                fileCount += file->read((char *)buff, YMODEM_PACKET_1K_SIZE);

                *len = YMODEM_PACKET_1K_SIZE;
            }
            else
            {
                fileCount += file->read((char *)buff, YMODEM_PACKET_SIZE);

                *len = YMODEM_PACKET_SIZE;
            }

            progress = (int)(fileCount * 100 / fileSize);

            YmodemSend::status = StatusTransmit;

            transmitProgress(progress);
            transmitStatus(StatusTransmit);

            return CodeAck;
        }
        else
        {
            YmodemSend::status = StatusTransmit;

            transmitStatus(StatusTransmit);

            return CodeEot;
        }
    }

    case StatusFinish:
    {
        file->close();

        YmodemSend::status = StatusFinish;

        qDebug()<<QStringLiteral("文件传输结束，结束状态");
        transmitDebugMessge(QStringLiteral("文件传输结束，结束状态\n"));
        writeTimer->start(WRITE_TIME_OUT);

        return CodeAck;
    }

    case StatusAbort:
    {
        file->close();
        YmodemSend::status = StatusAbort;
        qDebug()<<QStringLiteral("StatusAbort");
        transmitDebugMessge(QStringLiteral("StatusAbort\n"));
        writeTimer->start(WRITE_TIME_OUT);
        return CodeCan;
    }

    case StatusTimeout:
    {
        YmodemSend::status = StatusTimeout;
        qDebug()<<QStringLiteral("打开文件失败StatusTimeout");
        transmitDebugMessge(QStringLiteral("打开文件失败StatusTimeout\n"));
        writeTimer->start(WRITE_TIME_OUT);

        return CodeCan;
    }

    default:
    {
        file->close();

        YmodemSend::status = StatusError;

        qDebug()<<QStringLiteral("打开文件失败StatusError");
        transmitDebugMessge(QStringLiteral("Qt打开文件失败\n"));
        writeTimer->start(WRITE_TIME_OUT);

        return CodeCan;
    }
    }
}

uint32_t YmodemSend::read(uint8_t *buff, uint32_t len)
{ 

    return serialPort->read((char *)buff, len);


}

uint32_t YmodemSend::write(uint8_t *buff, uint32_t len)
{
    return serialPort->write((char *)buff, len);

}



