/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *combo_uart;
    QComboBox *combo_baud;
    QLabel *label_2;
    QPushButton *pushButton;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QTextBrowser *textBrowser;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_cleaar;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QPushButton *btn_file;
    QLineEdit *lineEdit;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QPushButton *btn_menu;
    QPushButton *btn_update;
    QPushButton *btn_write;
    QPushButton *btn_copy;
    QPushButton *btn_erase;
    QPushButton *btn_run;
    QProgressBar *progressBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(660, 464);
        QIcon icon;
        icon.addFile(QStringLiteral(":/src/write2file.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        combo_uart = new QComboBox(groupBox);
        combo_uart->setObjectName(QStringLiteral("combo_uart"));

        gridLayout->addWidget(combo_uart, 0, 1, 1, 1);

        combo_baud = new QComboBox(groupBox);
        combo_baud->setObjectName(QStringLiteral("combo_baud"));

        gridLayout->addWidget(combo_baud, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(8);
        pushButton->setFont(font);

        gridLayout->addWidget(pushButton, 2, 0, 1, 2);


        gridLayout_5->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        textBrowser = new QTextBrowser(groupBox_4);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout_4->addWidget(textBrowser, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 0, 1, 1);

        btn_cleaar = new QPushButton(groupBox_4);
        btn_cleaar->setObjectName(QStringLiteral("btn_cleaar"));

        gridLayout_4->addWidget(btn_cleaar, 1, 1, 1, 1);


        gridLayout_5->addWidget(groupBox_4, 0, 1, 2, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        btn_file = new QPushButton(groupBox_2);
        btn_file->setObjectName(QStringLiteral("btn_file"));

        gridLayout_2->addWidget(btn_file, 0, 1, 1, 1);

        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout_2->addWidget(lineEdit, 1, 0, 1, 2);


        gridLayout_5->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        btn_menu = new QPushButton(groupBox_3);
        btn_menu->setObjectName(QStringLiteral("btn_menu"));

        gridLayout_3->addWidget(btn_menu, 2, 0, 1, 1);

        btn_update = new QPushButton(groupBox_3);
        btn_update->setObjectName(QStringLiteral("btn_update"));

        gridLayout_3->addWidget(btn_update, 2, 1, 1, 1);

        btn_write = new QPushButton(groupBox_3);
        btn_write->setObjectName(QStringLiteral("btn_write"));

        gridLayout_3->addWidget(btn_write, 1, 0, 1, 1);

        btn_copy = new QPushButton(groupBox_3);
        btn_copy->setObjectName(QStringLiteral("btn_copy"));

        gridLayout_3->addWidget(btn_copy, 1, 1, 1, 1);

        btn_erase = new QPushButton(groupBox_3);
        btn_erase->setObjectName(QStringLiteral("btn_erase"));

        gridLayout_3->addWidget(btn_erase, 2, 3, 1, 1);

        btn_run = new QPushButton(groupBox_3);
        btn_run->setObjectName(QStringLiteral("btn_run"));

        gridLayout_3->addWidget(btn_run, 1, 3, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 2, 0, 1, 2);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        gridLayout_5->addWidget(progressBar, 3, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setIconSize(QSize(24, 24));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "  \344\270\262\345\217\243", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", " \346\263\242\347\211\271\347\216\207", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\344\277\241\346\201\257", Q_NULLPTR));
        btn_cleaar->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266\350\256\276\347\275\256", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\350\267\257\345\276\204", Q_NULLPTR));
        btn_file->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275\346\214\211\351\224\256", Q_NULLPTR));
        btn_menu->setText(QApplication::translate("MainWindow", "\344\270\273\350\217\234\345\215\225", Q_NULLPTR));
        btn_update->setText(QApplication::translate("MainWindow", "\345\215\207\347\272\247\347\250\213\345\272\217", Q_NULLPTR));
        btn_write->setText(QApplication::translate("MainWindow", "\345\206\231\345\215\207\347\272\247\346\240\207\345\277\227", Q_NULLPTR));
        btn_copy->setText(QApplication::translate("MainWindow", "\345\244\207\344\273\275\347\250\213\345\272\217", Q_NULLPTR));
        btn_erase->setText(QApplication::translate("MainWindow", "\346\223\246\351\231\244\347\250\213\345\272\217", Q_NULLPTR));
        btn_run->setText(QApplication::translate("MainWindow", "\350\277\220\350\241\214\347\250\213\345\272\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
