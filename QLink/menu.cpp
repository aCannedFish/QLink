// menu.cpp
// 主菜单界面实现文件
// 实现主菜单窗口的功能，包括界面初始化、音乐控制和模式切换

#include <QWidget>
#include <QPalette>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "menu.h"
#include "simplemode.h"
#include "duomode.h"
#include "load.h"
#include <QFileDialog>
#include <QMessageBox>

// 主菜单构造函数
// parent: 父窗口指针
// 初始化主菜单界面，包括窗口属性、背景、按钮样式、音乐播放器和信号槽
Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuClass())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    ui->playControlBtn->setStyleSheet("QPushButton{border-image: url(:/images/images/itsmygo.png);}");
    setStyleSheet("Menu { background-image: url(:/images/images/background.png); }");
    ui->appTitle->setStyleSheet("QLabel { color: rgb(147, 218, 100); }");
    ui->appTitle2->setStyleSheet("QLabel { color: rgba(255,255,255,204); }");
    ui->simpleModeBtn->setStyleSheet("color: rgb(147, 218, 100);");
    ui->duoModeBtn->setStyleSheet("color: rgb(147, 218, 100);");
    ui->loadBtn->setStyleSheet("color: rgb(147, 218, 100);");
    ui->exitBtn->setStyleSheet("color: rgb(147, 218, 100);");
    mediaPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl("qrc:/images/music/music2.mp3"));
    mediaPlayer->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(0.5);
    mediaPlayer->play();
    connect(ui->playControlBtn, &QPushButton::clicked, this, &Menu::playControlSlot);
    connect(ui->simpleModeBtn, &QPushButton::clicked, this, &Menu::simpleModeSlot);
    connect(ui->duoModeBtn, &QPushButton::clicked, this, &Menu::duoModeSlot);
    connect(ui->loadBtn, &QPushButton::clicked, this, &Menu::loadSlot);
}

// 主菜单析构函数
// 清理资源，停止背景音乐播放
Menu::~Menu()
{
    mediaPlayer->stop();
    delete ui;
}

// 音乐播放控制函数
// 控制背景音乐的播放/暂停状态
void Menu::playControlSlot()
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
    } else {
        mediaPlayer->play();
    }
}

// 单机模式启动函数
// 创建并显示单机模式游戏窗口
void Menu::simpleModeSlot()
{
    SimpleMode* simpleModeWindow = new SimpleMode(this);
    connect(simpleModeWindow, &SimpleMode::exitToMenu, this, [this, simpleModeWindow]() {
        this->show();
        simpleModeWindow->deleteLater();
    });
    simpleModeWindow->show();
}

// 双人模式启动函数
// 创建并显示双人模式游戏窗口
void Menu::duoModeSlot()
{
    DuoMode* duoModeWindow = new DuoMode(this);
    connect(duoModeWindow, &DuoMode::exitToMenu, this, [this, duoModeWindow]() {
        this->show();
        duoModeWindow->deleteLater();
    });
    duoModeWindow->show();
}

// 读取存档函数
// 读取游戏存档并启动对应的游戏模式
void Menu::loadSlot() {
    QString path = QFileDialog::getOpenFileName(this, "读取存档", "", "存档文件 (*.txt)");
    if (path.isEmpty()) return;
    SaveData data;
    if (!loadGame(path, data)) {
        QMessageBox::warning(this, "错误", "存档读取失败！");
        return;
    }
    if (data.mode == GameMode::Single) {
        SimpleMode* simpleModeWindow = new SimpleMode(this, &data);
        connect(simpleModeWindow, &SimpleMode::exitToMenu, this, [this, simpleModeWindow]() {
            this->show();
            simpleModeWindow->deleteLater();
        });
        simpleModeWindow->show();
    } else if (data.mode == GameMode::Duo) {
        DuoMode* duoModeWindow = new DuoMode(this, &data);
        connect(duoModeWindow, &DuoMode::exitToMenu, this, [this, duoModeWindow]() {
            this->show();
            duoModeWindow->deleteLater();
        });
        duoModeWindow->show();
    }
}

