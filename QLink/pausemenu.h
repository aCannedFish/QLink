#pragma once

#include <QDialog>
#include "ui_pausemenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PauseMenuClass; };
QT_END_NAMESPACE

// 游戏暂停菜单类
// 继承自QDialog，提供游戏暂停时的功能选择界面
// 包含继续游戏、退出游戏、保存游戏、加载游戏等选项
class PauseMenu : public QDialog
{
    Q_OBJECT

public:
    // 构造函数
    // parent: 父窗口指针，默认为nullptr
    // 初始化暂停菜单界面，设置窗口属性和信号槽连接
    PauseMenu(QWidget *parent = nullptr);
    
    // 析构函数
    // 清理暂停菜单资源
    ~PauseMenu();

signals:
    // 继续游戏信号
    // 当用户点击继续按钮时发出，通知主游戏窗口恢复游戏
    void continueClicked();
    
    // 退出游戏信号
    // 当用户点击退出按钮时发出，通知主游戏窗口退出到主菜单
    void exitClicked();
    
    // 保存游戏信号
    // 当用户点击保存按钮时发出，通知主游戏窗口保存当前游戏状态
    void saveClicked();
    
    // 加载游戏信号
    // 当用户点击加载按钮时发出，通知主游戏窗口加载存档
    void loadClicked();

private:
    Ui::PauseMenuClass *ui; // UI界面指针，管理暂停菜单的所有控件
};

