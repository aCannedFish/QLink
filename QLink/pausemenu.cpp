#include "pausemenu.h"
#include <QCloseEvent>

PauseMenu::PauseMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PauseMenuClass())
{
    // 设置UI界面
    ui->setupUi(this);
    
    // 禁止通过右上角关闭按钮关闭窗口
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    
    // 设置为应用程序模态对话框，阻止用户与其他窗口交互
    setWindowModality(Qt::ApplicationModal);
    
    // 再次确保关闭按钮被禁用
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);

    // 连接按钮点击信号到对应的槽函数
    connect(ui->continueBtn, &QPushButton::clicked, this, &PauseMenu::continueClicked);
    connect(ui->exitBtn, &QPushButton::clicked, this, &PauseMenu::exitClicked);
    connect(ui->saveBtn, &QPushButton::clicked, this, &PauseMenu::saveClicked);
    connect(ui->loadBtn, &QPushButton::clicked, this, &PauseMenu::loadClicked);
}


// 暂停菜单析构函数
// 清理暂停菜单资源，删除UI对象
 
PauseMenu::~PauseMenu()
{
    delete ui; // 删除UI对象
}

