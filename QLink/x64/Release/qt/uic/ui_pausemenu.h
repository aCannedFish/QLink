/********************************************************************************
** Form generated from reading UI file 'pausemenu.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAUSEMENU_H
#define UI_PAUSEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PauseMenuClass
{
public:
    QPushButton *continueBtn;
    QPushButton *saveBtn;
    QPushButton *loadBtn;
    QPushButton *exitBtn;

    void setupUi(QDialog *PauseMenuClass)
    {
        if (PauseMenuClass->objectName().isEmpty())
            PauseMenuClass->setObjectName("PauseMenuClass");
        PauseMenuClass->resize(400, 300);
        PauseMenuClass->setMinimumSize(QSize(400, 300));
        PauseMenuClass->setMaximumSize(QSize(400, 300));
        continueBtn = new QPushButton(PauseMenuClass);
        continueBtn->setObjectName("continueBtn");
        continueBtn->setGeometry(QRect(40, 20, 321, 51));
        saveBtn = new QPushButton(PauseMenuClass);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setGeometry(QRect(40, 160, 321, 51));
        loadBtn = new QPushButton(PauseMenuClass);
        loadBtn->setObjectName("loadBtn");
        loadBtn->setGeometry(QRect(40, 230, 321, 51));
        exitBtn = new QPushButton(PauseMenuClass);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(40, 90, 321, 51));

        retranslateUi(PauseMenuClass);

        QMetaObject::connectSlotsByName(PauseMenuClass);
    } // setupUi

    void retranslateUi(QDialog *PauseMenuClass)
    {
        PauseMenuClass->setWindowTitle(QCoreApplication::translate("PauseMenuClass", "PauseMenu", nullptr));
        continueBtn->setText(QCoreApplication::translate("PauseMenuClass", "\347\273\247\347\273\255\346\270\270\346\210\217", nullptr));
        saveBtn->setText(QCoreApplication::translate("PauseMenuClass", "\344\277\235\345\255\230\345\255\230\346\241\243", nullptr));
        loadBtn->setText(QCoreApplication::translate("PauseMenuClass", "\350\257\273\345\217\226\345\255\230\346\241\243", nullptr));
        exitBtn->setText(QCoreApplication::translate("PauseMenuClass", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PauseMenuClass: public Ui_PauseMenuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAUSEMENU_H
