// main.cpp
// QLink连连看游戏主程序入口文件
// 程序启动入口，负责初始化Qt应用程序并显示主菜单界面

#include "menu.h"
#include <QtWidgets/QApplication>

// 程序主函数
// argc: 命令行参数个数
// argv: 命令行参数数组
// 返回值: 程序退出码
int main(int argc, char *argv[])
{
    // 创建Qt应用程序实例，处理命令行参数
    QApplication app(argc, argv);
    // 创建主菜单窗口实例
    Menu window;
    // 显示主菜单窗口
    window.show();
    // 进入Qt事件循环，处理用户交互直到程序退出
    return app.exec();
}
