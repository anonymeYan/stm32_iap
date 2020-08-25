#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize ( 660, 460 );
    setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
    setWindowTitle(QStringLiteral("电池管理系统固件升级平台V2.0"));

    serialPort = new QSerialPort;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->combo_uart->addItem(serial.portName());
            serial.close();
        }
    }

    ui->combo_baud->addItem("9600");
    ui->combo_baud->addItem("115200");
    ui->statusBar->showMessage(QStringLiteral("临时信息!"),1000);
    ui->btn_menu->setDisabled(true);
    ui->btn_erase->setDisabled(true);
    ui->btn_update->setDisabled(true);
    ui->btn_run->setDisabled(true);
    ui->btn_write->setDisabled(true);
    ui->btn_copy->setDisabled(true);

    ymodemSend = new YmodemSend;
    connect(ymodemSend, SIGNAL(transmitProgress(int)), this, SLOT(transmitProgress(int)));
    connect(ymodemSend, SIGNAL(transmitStatus(YmodemSend::Status)), this, SLOT(transmitStatus(YmodemSend::Status)));
    connect(ymodemSend, SIGNAL(transmitDebugMessge(const QString& )), this, SLOT(transmitDebugMessge(const QString&)));

//    transmitButtonStatus = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if (!serialPort){
        statusBar()->showMessage(QStringLiteral("串口不存在"), 5000);
        return;
    }
    statusBar()->clearMessage();

    QString comPortName = ui->combo_uart->currentText();
    int Baud = ui->combo_baud->currentText().toInt();

    serialPort->setPortName(comPortName);
    serialPort->setBaudRate(Baud);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if(button_status == false)
    {
        if(serialPort->open(QSerialPort::ReadWrite) == true)
        {
            button_status = true;
            ui->combo_uart->setDisabled(true);
            ui->combo_baud->setDisabled(true);
            ui->pushButton->setText(QStringLiteral("关闭串口"));
            //             connect(serialPort,SIGNAL(readyRead()),this,SLOT(SerialRecvMsgEvent()));
            QObject::connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::SerialRecvMsgEvent);
        }
        else
        {
            ui->statusBar->showMessage(QStringLiteral("串口打开失败!"),5000);
        }
    }
    else
    {
        button_status = false;
        serialPort->close();
        ui->combo_uart->setEnabled(true);
        ui->combo_baud->setEnabled(true);
        ui->pushButton->setText(QStringLiteral("打开串口"));
    }
}


void MainWindow::SerialRecvMsgEvent()
{
    if(serialPort->bytesAvailable() >= 0)
    {
        QByteArray buf;
        QString np;
        buf = serialPort->readAll();
        qDebug() << "size: " << buf.size()<< "data: " << buf;
        np  = QString::fromLocal8Bit(buf);
        if(!buf.isEmpty())
        {
            ui->textBrowser->insertPlainText(np);
            ui->textBrowser->moveCursor(QTextCursor::End);//接收框始终定为在末尾一行
        }
    }
    else
    {
        qDebug()<<"接受数据出错" + QString::number(serialPort->bytesAvailable());
    }
}


void MainWindow::on_btn_file_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this, QStringLiteral("选择bin文件"), ".",QStringLiteral("文件 (*.bin)")));
    QFileInfo fileInfo(ui->lineEdit->text());
    ui->textBrowser->insertPlainText(fileInfo.fileName());
    ui->textBrowser->insertPlainText("      ");
    ui->textBrowser->insertPlainText(QString::number(fileInfo.size()));
    ui->textBrowser->insertPlainText("Byte");
    ui->textBrowser->moveCursor(QTextCursor::End);//接收框始终定为在末尾一行

    if(ui->lineEdit->text().isEmpty() != true)
    {
        ui->btn_menu->setEnabled(true);
        ui->btn_erase->setEnabled(true);
        ui->btn_update->setEnabled(true);
        ui->btn_run->setEnabled(true);
        ui->btn_write->setEnabled(true);
        ui->btn_copy->setEnabled(true);
    }
    else
    {
        ui->btn_menu->setDisabled(true);
        ui->btn_erase->setDisabled(true);
        ui->btn_update->setDisabled(true);
        ui->btn_run->setDisabled(true);
        ui->btn_write->setDisabled(true);
    }

}





void MainWindow::on_btn_cleaar_clicked()
{
    ui->textBrowser->clear();
}



void MainWindow::on_btn_menu_clicked()
{
    if(serialPort->isOpen())
    {
        QString cmd = "menu\r\n";
        QByteArray sendData = cmd.toLatin1();
        serialPort->write(sendData);
    }

}

void MainWindow::on_btn_copy_clicked()
{
    if(serialPort->isOpen())
    {
        QString cmd = "copyapp\r\n";
        QByteArray sendData = cmd.toLatin1();
        serialPort->write(sendData);
    }
}

void MainWindow::on_btn_erase_clicked()
{
    if(serialPort->isOpen())
    {
        QString cmd = "erase\r\n";
        QByteArray sendData = cmd.toLatin1();
        serialPort->write(sendData);
    }

}

void MainWindow::on_btn_update_clicked()
{
    if(serialPort->isOpen())
    {
        QString cmd = "update\r\n";
        QByteArray sendData = cmd.toLatin1();
        serialPort->write(sendData);
    }
    Delay_MSec(2000);
    if(transmitButtonStatus == false)
    {
        serialPort->close();

        ymodemSend->setFileName(ui->lineEdit->text());
        ymodemSend->setPortName(ui->combo_uart->currentText());
        ymodemSend->setPortBaudRate(ui->combo_baud->currentText().toInt());
        //        connect(ymodemSend, SIGNAL(ymodemsendSerialPotrSignal(QByteArray data)), this,  SLOT(ReceiveFromYsend(QByteArray data)));
        if(ymodemSend->startTransmit() == true)
        {
            qDebug()<<QStringLiteral("开始传输");
            transmitButtonStatus = true;
            ui->progressBar->setValue(0);
        }
        else
        {
            qDebug()<<QStringLiteral("开始传输失败");
            ui->statusBar->showMessage(QStringLiteral("开始传输失败!"),1000);
        }
    }
    else
    {
        qDebug()<<QStringLiteral("transmitButtonStatus=true stopTransmit ");
        ymodemSend->stopTransmit();
    }


}

void MainWindow::on_btn_run_clicked()
{
    if(serialPort->isOpen())
    {
        QString cmd = "runapp\r\n";
        QByteArray sendData = cmd.toLatin1();
        serialPort->write(sendData);
    }

}

void MainWindow::on_btn_write_clicked()
{
    if(serialPort->isOpen())
    {
        QByteArray sendData ;
        sendData.resize(8);
        sendData[0]=0x01;
        sendData[1]=0x06;
        sendData[2]=0x00;
        sendData[3]=0x63;
        sendData[4]=0x00;
        sendData[5]=0x01;
        sendData[6]='\x0b8';
        sendData[7]=0x14;
        serialPort->write(sendData);
    }

}

// 函数 string 转为 16进制
void MainWindow::convertStringToHex(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertCharToHex(hstr);
        lowhexdata = convertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] =/*static_cast<char>(hexdata)*/ (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}


// 函数 char 转为 16进制
char MainWindow::convertCharToHex(char ch)
{
    /*
    0x30等于十进制的48，48也是0的ASCII值，，
    1-9的ASCII值是49-57，，所以某一个值－0x30，，
    就是将字符0-9转换为0-9

    */
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}



void MainWindow::transmitDebugMessge(const QString& Messge)
{
    if(Messge==QStringLiteral("关闭文件发送串口\n"))
    {
        button_status = false;
        ui->combo_uart->setEnabled(true);
        ui->combo_baud->setEnabled(true);
        ui->pushButton->setText(QStringLiteral("打开串口"));
        if(serialPort->open(QSerialPort::ReadWrite) == true)
        {
            button_status = true;
            ui->combo_uart->setDisabled(true);
            ui->combo_baud->setDisabled(true);
            ui->pushButton->setText(QStringLiteral("关闭串口"));
            QObject::connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::SerialRecvMsgEvent);
        }
        else
        {
            ui->statusBar->showMessage(QStringLiteral("串口打开失败!"),5000);
        }
    }
    ui->textBrowser->insertPlainText(Messge);
    ui->textBrowser->moveCursor(QTextCursor::End);//接收框始终定为在末尾一行
}


void MainWindow::transmitProgress(int progress)
{
    ui->progressBar->setValue(progress);
}


void MainWindow::transmitStatus(Ymodem::Status status)
{
    switch(status)
    {
    case YmodemSend::StatusEstablish:
    {
        break;
    }

    case YmodemSend::StatusTransmit:
    {
        break;
    }

    case YmodemSend::StatusFinish:
    {
//        ui->btn_sendfile->setEnabled(true);
        transmitButtonStatus = false;
        ui->textBrowser->insertPlainText(QStringLiteral(" 文件发送完成！"));
        ui->textBrowser->moveCursor(QTextCursor::End);//接收框始终定为在末尾一行
        qDebug()<<QStringLiteral("文件发送完成！");
        QMessageBox::warning(this, QStringLiteral("成功"), QStringLiteral("文件发送成功！"), QStringLiteral("关闭"));
        break;
    }

    case YmodemSend::StatusAbort:
    {
//        ui->btn_sendfile->setEnabled(true);
        transmitButtonStatus = false;
        QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("文件发送失败1！"), QStringLiteral("关闭"));
        break;
    }

    case YmodemSend::StatusTimeout:
    {
//        ui->btn_sendfile->setEnabled(true);
        transmitButtonStatus = false;
        QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("文件发送失败2！"), QStringLiteral("关闭"));
        break;
    }

    default:
    {
//        ui->btn_sendfile->setEnabled(true);
        transmitButtonStatus = false;
        QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("文件发送失败3！"), QStringLiteral("关闭"));
        break;
    }
    }
}


QString  MainWindow::fromGBKtoUtf8(QByteArray arry)
{
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString unicode=gbk->toUnicode(arry);
    return QString(utf8->fromUnicode(unicode));
}

QByteArray MainWindow::fromUtf8toGBK(QString str)
{
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QString unicode=utf8->toUnicode(str.toLocal8Bit());
    return gbk->fromUnicode(unicode);
}



void MainWindow::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}


