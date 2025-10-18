/********************************************************************************
** Form generated from reading UI file 'duomode.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DUOMODE_H
#define UI_DUOMODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DuoModeClass
{
public:
    QWidget *centralWidget;
    QPushButton *exitBtn;
    QPushButton *pauseBtn;

    void setupUi(QMainWindow *DuoModeClass)
    {
        if (DuoModeClass->objectName().isEmpty())
            DuoModeClass->setObjectName("DuoModeClass");
        DuoModeClass->setEnabled(true);
        DuoModeClass->resize(1200, 800);
        centralWidget = new QWidget(DuoModeClass);
        centralWidget->setObjectName("centralWidget");
        exitBtn = new QPushButton(centralWidget);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(970, 17, 101, 46));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setItalic(true);
        exitBtn->setFont(font);
        pauseBtn = new QPushButton(centralWidget);
        pauseBtn->setObjectName("pauseBtn");
        pauseBtn->setGeometry(QRect(1084, 17, 101, 46));
        pauseBtn->setFont(font);
        DuoModeClass->setCentralWidget(centralWidget);

        retranslateUi(DuoModeClass);

        QMetaObject::connectSlotsByName(DuoModeClass);
    } // setupUi

    void retranslateUi(QMainWindow *DuoModeClass)
    {
        DuoModeClass->setWindowTitle(QCoreApplication::translate("DuoModeClass", "DuoMode", nullptr));
        exitBtn->setText(QCoreApplication::translate("DuoModeClass", "\351\200\200\345\207\272", nullptr));
        pauseBtn->setText(QCoreApplication::translate("DuoModeClass", "\346\232\202\345\201\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DuoModeClass: public Ui_DuoModeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DUOMODE_H
