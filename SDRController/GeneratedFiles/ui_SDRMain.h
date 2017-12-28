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
#include <QtWidgets/QMainWindow>
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
    } // retranslateUi

};

namespace Ui {
    class SDRMainClass: public Ui_SDRMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDRMAIN_H
