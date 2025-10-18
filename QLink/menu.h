#pragma once

#include <QtWidgets/QMainWindow>
#include <QMediaPlayer>
#include "ui_menu.h"
#include "simplemode.h"
#include "duomode.h"
#include "load.h"
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuClass; };
QT_END_NAMESPACE

// 主菜单窗口类
// 继承自QMainWindow，提供游戏的主菜单界面
// 包含单机模式、双人模式、读取存档等功能入口
class Menu : public QMainWindow
{
    Q_OBJECT
 
public:
    // 构造函数
    // parent: 父窗口指针，默认为nullptr
    // 初始化主菜单界面，设置背景、按钮样式和背景音乐
    Menu(QWidget *parent = nullptr);
    
    // 析构函数
    // 清理资源，停止背景音乐播放
    ~Menu();

private slots:
    // 音乐播放控制槽函数
    // 控制背景音乐的播放/暂停状态
    void playControlSlot();
    
    // 单机模式槽函数
    // 创建并显示单机模式游戏窗口
    void simpleModeSlot();
    
    // 双人模式槽函数
    // 创建并显示双人模式游戏窗口
	void duoModeSlot();
	
	// 读取存档槽函数
	// 打开文件对话框选择存档文件，并加载游戏状态
	void loadSlot();

private:
    Ui::MenuClass *ui;        // UI界面指针，管理菜单界面的所有控件
    QMediaPlayer* mediaPlayer; // 媒体播放器指针，用于播放背景音乐
};

