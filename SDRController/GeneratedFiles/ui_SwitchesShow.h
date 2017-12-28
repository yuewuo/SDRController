/********************************************************************************
** Form generated from reading UI file 'SwitchesShow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SWITCHESSHOW_H
#define UI_SWITCHESSHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SwitchesShow
{
public:

    void setupUi(QWidget *SwitchesShow)
    {
        if (SwitchesShow->objectName().isEmpty())
            SwitchesShow->setObjectName(QStringLiteral("SwitchesShow"));
        SwitchesShow->resize(600, 600);

        retranslateUi(SwitchesShow);

        QMetaObject::connectSlotsByName(SwitchesShow);
    } // setupUi

    void retranslateUi(QWidget *SwitchesShow)
    {
        SwitchesShow->setWindowTitle(QApplication::translate("SwitchesShow", "SwitchesShow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SwitchesShow: public Ui_SwitchesShow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SWITCHESSHOW_H
