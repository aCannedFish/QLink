/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuClass
{
public:
    QWidget *centralWidget;
    QLabel *appTitle;
    QPushButton *simpleModeBtn;
    QPushButton *duoModeBtn;
    QPushButton *loadBtn;
    QPushButton *exitBtn;
    QPushButton *playControlBtn;
    QLabel *appTitle2;

    void setupUi(QMainWindow *MenuClass)
    {
        if (MenuClass->objectName().isEmpty())
            MenuClass->setObjectName("MenuClass");
        MenuClass->resize(597, 802);
        MenuClass->setMinimumSize(QSize(597, 802));
        MenuClass->setMaximumSize(QSize(597, 802));
        QFont font;
        font.setPointSize(9);
        font.setItalic(true);
        MenuClass->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("images/itsmygo.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MenuClass->setWindowIcon(icon);
        centralWidget = new QWidget(MenuClass);
        centralWidget->setObjectName("centralWidget");
        appTitle = new QLabel(centralWidget);
        appTitle->setObjectName("appTitle");
        appTitle->setGeometry(QRect(200, 40, 351, 101));
        QFont font1;
        font1.setPointSize(51);
        font1.setBold(true);
        font1.setItalic(true);
        appTitle->setFont(font1);
        simpleModeBtn = new QPushButton(centralWidget);
        simpleModeBtn->setObjectName("simpleModeBtn");
        simpleModeBtn->setGeometry(QRect(60, 250, 481, 81));
        QFont font2;
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setItalic(true);
        font2.setUnderline(false);
        simpleModeBtn->setFont(font2);
        duoModeBtn = new QPushButton(centralWidget);
        duoModeBtn->setObjectName("duoModeBtn");
        duoModeBtn->setGeometry(QRect(60, 380, 481, 81));
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(true);
        font3.setItalic(true);
        duoModeBtn->setFont(font3);
        loadBtn = new QPushButton(centralWidget);
        loadBtn->setObjectName("loadBtn");
        loadBtn->setGeometry(QRect(60, 510, 481, 81));
        loadBtn->setFont(font3);
        exitBtn = new QPushButton(centralWidget);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(60, 640, 481, 81));
        exitBtn->setFont(font3);
        playControlBtn = new QPushButton(centralWidget);
        playControlBtn->setObjectName("playControlBtn");
        playControlBtn->setGeometry(QRect(60, 60, 141, 141));
        QFont font4;
        font4.setItalic(true);
        playControlBtn->setFont(font4);
        appTitle2 = new QLabel(centralWidget);
        appTitle2->setObjectName("appTitle2");
        appTitle2->setGeometry(QRect(230, 150, 161, 51));
        QFont font5;
        font5.setPointSize(44);
        font5.setBold(true);
        font5.setItalic(true);
        appTitle2->setFont(font5);
        MenuClass->setCentralWidget(centralWidget);

        retranslateUi(MenuClass);
        QObject::connect(exitBtn, &QPushButton::clicked, MenuClass, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(MenuClass);
    } // setupUi

    void retranslateUi(QMainWindow *MenuClass)
    {
        MenuClass->setWindowTitle(QCoreApplication::translate("MenuClass", "Menu", nullptr));
        appTitle->setText(QCoreApplication::translate("MenuClass", "Minecraft", nullptr));
        simpleModeBtn->setText(QCoreApplication::translate("MenuClass", "\345\215\225\344\272\272\346\250\241\345\274\217", nullptr));
        duoModeBtn->setText(QCoreApplication::translate("MenuClass", "\345\217\214\344\272\272\346\250\241\345\274\217", nullptr));
        loadBtn->setText(QCoreApplication::translate("MenuClass", "\350\257\273\345\217\226\345\255\230\346\241\243", nullptr));
        exitBtn->setText(QCoreApplication::translate("MenuClass", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
        playControlBtn->setText(QString());
        appTitle2->setText(QCoreApplication::translate("MenuClass", "Link", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MenuClass: public Ui_MenuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
