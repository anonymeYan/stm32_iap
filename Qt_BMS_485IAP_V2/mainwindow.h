#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QDebug"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>
#include "ymodemsend.h"
#include <QMessageBox>
#include "qtextcodec.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Delay_MSec(unsigned int msec);

private slots:
    void on_pushButton_clicked();

    void on_btn_file_clicked();

    void on_btn_cleaar_clicked();

    void on_btn_menu_clicked();

    void on_btn_erase_clicked();

    void on_btn_update_clicked();

    void on_btn_run_clicked();

    void on_btn_write_clicked();

    void SerialRecvMsgEvent();

    void transmitProgress(int progress);

    void transmitStatus(YmodemSend::Status status);
    void transmitDebugMessge(const QString& Messge);

    void on_btn_copy_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    YmodemSend *ymodemSend;
    bool transmitButtonStatus;
    bool button_status = false;
    char convertCharToHex(char ch);
    void convertStringToHex(const QString &str, QByteArray &byteData);
    QString  fromGBKtoUtf8(QByteArray arry);
    QByteArray fromUtf8toGBK(QString str);

};

#endif // MAINWINDOW_H
