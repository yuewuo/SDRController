/********************************************************************************
** Form generated from reading UI file 'SDRMain.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SDRMAIN_H
#define UI_SDRMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "SwitchesShow.h"

QT_BEGIN_NAMESPACE

class Ui_SDRMainClass
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    SwitchesShow *switchesShowWidget;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_importconfig;
    QPushButton *pushButton_exportconfig;
    QPushButton *pushButton_changLine;
    QLineEdit *lineEdit_changLinex1;
    QLineEdit *lineEdit_changLiney1;
    QLineEdit *lineEdit_changLiney2;
    QLineEdit *lineEdit_changLinestat;
    QLineEdit *lineEdit_changLinex2;
    QLineEdit *lineEdit_changLinenum;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SDRMainClass)
    {
        if (SDRMainClass->objectName().isEmpty())
            SDRMainClass->setObjectName(QStringLiteral("SDRMainClass"));
        SDRMainClass->resize(1600, 900);
        centralWidget = new QWidget(SDRMainClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 600, 861));
        switchesShowWidget = new SwitchesShow(groupBox);
        switchesShowWidget->setObjectName(QStringLiteral("switchesShowWidget"));
        switchesShowWidget->setGeometry(QRect(0, 20, 600, 600));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(620, 10, 361, 411));
        pushButton_importconfig = new QPushButton(groupBox_2);
        pushButton_importconfig->setObjectName(QStringLiteral("pushButton_importconfig"));
        pushButton_importconfig->setGeometry(QRect(10, 20, 341, 31));
        pushButton_exportconfig = new QPushButton(groupBox_2);
        pushButton_exportconfig->setObjectName(QStringLiteral("pushButton_exportconfig"));
        pushButton_exportconfig->setGeometry(QRect(10, 60, 341, 31));
        pushButton_changLine = new QPushButton(groupBox_2);
        pushButton_changLine->setObjectName(QStringLiteral("pushButton_changLine"));
        pushButton_changLine->setGeometry(QRect(250, 100, 101, 31));
        lineEdit_changLinex1 = new QLineEdit(groupBox_2);
        lineEdit_changLinex1->setObjectName(QStringLiteral("lineEdit_changLinex1"));
        lineEdit_changLinex1->setGeometry(QRect(10, 100, 31, 31));
        lineEdit_changLiney1 = new QLineEdit(groupBox_2);
        lineEdit_changLiney1->setObjectName(QStringLiteral("lineEdit_changLiney1"));
        lineEdit_changLiney1->setGeometry(QRect(50, 100, 31, 31));
        lineEdit_changLiney2 = new QLineEdit(groupBox_2);
        lineEdit_changLiney2->setObjectName(QStringLiteral("lineEdit_changLiney2"));
        lineEdit_changLiney2->setGeometry(QRect(130, 100, 31, 31));
        lineEdit_changLinestat = new QLineEdit(groupBox_2);
        lineEdit_changLinestat->setObjectName(QStringLiteral("lineEdit_changLinestat"));
        lineEdit_changLinestat->setGeometry(QRect(210, 100, 31, 31));
        lineEdit_changLinex2 = new QLineEdit(groupBox_2);
        lineEdit_changLinex2->setObjectName(QStringLiteral("lineEdit_changLinex2"));
        lineEdit_changLinex2->setGeometry(QRect(90, 100, 31, 31));
        lineEdit_changLinenum = new QLineEdit(groupBox_2);
        lineEdit_changLinenum->setObjectName(QStringLiteral("lineEdit_changLinenum"));
        lineEdit_changLinenum->setGeometry(QRect(170, 100, 31, 31));
        SDRMainClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SDRMainClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SDRMainClass->setStatusBar(statusBar);

        retranslateUi(SDRMainClass);

        QMetaObject::connectSlotsByName(SDRMainClass);
    } // setupUi

    void retranslateUi(QMainWindow *SDRMainClass)
    {
        SDRMainClass->setWindowTitle(QApplication::translate("SDRMainClass", "SDRMain", nullptr));
        groupBox->setTitle(QApplication::translate("SDRMainClass", "Status", nullptr));
        groupBox_2->setTitle(QApplication::translate("SDRMainClass", "Tools", nullptr));
        pushButton_importconfig->setText(QApplication::translate("SDRMainClass", "Import Configuration (from switches.json)", nullptr));
        pushButton_exportconfig->setText(QApplication::translate("SDRMainClass", "Export Configuration (to switches.json)", nullptr));
        pushButton_changLine->setText(QApplication::translate("SDRMainClass", "changeLine", nullptr));
        lineEdit_changLinex1->setText(QApplication::translate("SDRMainClass", "x1", nullptr));
        lineEdit_changLiney1->setText(QApplication::translate("SDRMainClass", "y1", nullptr));
        lineEdit_changLiney2->setText(QApplication::translate("SDRMainClass", "y2", nullptr));
        lineEdit_changLinestat->setText(QApplication::translate("SDRMainClass", "stat", nullptr));
        lineEdit_changLinex2->setText(QApplication::translate("SDRMainClass", "x2", nullptr));
        lineEdit_changLinenum->setText(QApplication::translate("SDRMainClass", "num", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SDRMainClass: public Ui_SDRMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDRMAIN_H
