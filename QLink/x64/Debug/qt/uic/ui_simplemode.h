/********************************************************************************
** Form generated from reading UI file 'simplemode.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLEMODE_H
#define UI_SIMPLEMODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimpleModeClass
{
public:
    QWidget *centralWidget;
    QPushButton *pauseBtn;
    QPushButton *exitBtn;

    void setupUi(QMainWindow *SimpleModeClass)
    {
        if (SimpleModeClass->objectName().isEmpty())
            SimpleModeClass->setObjectName("SimpleModeClass");
        SimpleModeClass->resize(1200, 800);
        SimpleModeClass->setMinimumSize(QSize(1200, 800));
        SimpleModeClass->setMaximumSize(QSize(1200, 800));
        centralWidget = new QWidget(SimpleModeClass);
        centralWidget->setObjectName("centralWidget");
        pauseBtn = new QPushButton(centralWidget);
        pauseBtn->setObjectName("pauseBtn");
        pauseBtn->setGeometry(QRect(1084, 17, 101, 46));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setItalic(true);
        pauseBtn->setFont(font);
        exitBtn = new QPushButton(centralWidget);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(970, 17, 101, 46));
        exitBtn->setFont(font);
        SimpleModeClass->setCentralWidget(centralWidget);

        retranslateUi(SimpleModeClass);
        QObject::connect(exitBtn, &QPushButton::clicked, SimpleModeClass, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(SimpleModeClass);
    } // setupUi

    void retranslateUi(QMainWindow *SimpleModeClass)
    {
        SimpleModeClass->setWindowTitle(QCoreApplication::translate("SimpleModeClass", "SimpleMode", nullptr));
        pauseBtn->setText(QCoreApplication::translate("SimpleModeClass", "\346\232\202\345\201\234", nullptr));
        exitBtn->setText(QCoreApplication::translate("SimpleModeClass", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleModeClass: public Ui_SimpleModeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLEMODE_H
