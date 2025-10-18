// simplemode.h
// 单机模式游戏窗口头文件
// 定义连连看游戏单机模式的窗口类，包含游戏逻辑、方块管理、道具系统等

#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QProgressBar>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QLabel>
#include <array>
#include "block.h"
#include "player.h"
#include "ui_simplemode.h"
#include "item.h"
#include "load.h"
#include "pausemenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SimpleModeClass; }
QT_END_NAMESPACE

// 单机模式游戏窗口类
// 继承自QMainWindow，实现连连看游戏的单机模式
// 包含完整的游戏逻辑：方块管理、路径查找、道具系统、计分系统等
class SimpleMode : public QMainWindow
{
    Q_OBJECT
public:
    // 声明测试类为友元类
    friend class SimpleTest;
    // 构造函数
    // parent: 父窗口指针，默认为nullptr
    // saveData: 存档数据指针，用于加载游戏状态，默认为nullptr
    // 初始化单机模式游戏窗口，设置游戏界面和逻辑
    SimpleMode(QWidget *parent = nullptr, const SaveData* saveData = nullptr);
    
    // 析构函数
    // 清理游戏资源，停止定时器，删除动态分配的对象
    ~SimpleMode();
    
    // 洗牌功能
    // 重新排列所有方块，打乱游戏布局
    void shuffle();
    
    // 游戏进度更新
    // 更新游戏状态，包括时间、分数等
    void progress();

signals:
    // 游戏胜利信号
    // 当玩家成功消除所有方块时发出
    void win();
    
    // 时间耗尽信号
    // 当游戏时间用完时发出
    void timeOut();
    
    // 退出到主菜单信号
    // 当玩家选择退出游戏时发出
    void exitToMenu();

private slots:
    // 退出按钮点击槽函数
    // 处理退出按钮点击事件
    void on_exitBtn_clicked();
    
    // 暂停游戏
    // 暂停游戏并显示暂停菜单
    void pauseGame();
    
    // 恢复游戏
    // 从暂停状态恢复游戏
    void resumeGame();
    
    // 保存按钮点击槽函数
    // 处理保存游戏按钮点击事件
    void onSaveBtnClicked();
    
    // 加载按钮点击槽函数
    // 处理加载游戏按钮点击事件
    void onLoadBtnClicked();
    
    // 继续按钮点击槽函数
    // 处理继续游戏按钮点击事件
    void onContinueBtnClicked();
    
    // 退出按钮点击槽函数
    // 处理退出游戏按钮点击事件
    void onExitBtnClicked();
    
    // 获取当前游戏数据
    // return: 当前游戏状态的存档数据
    // 收集当前游戏的所有状态信息，用于保存游戏
    SaveData getSaveData() const;
    
    // 应用存档数据
    // data: 要应用的存档数据
    // 将存档数据应用到当前游戏，恢复游戏状态
    void applySaveData(const SaveData& data);

private:
    Ui::SimpleModeClass *ui;             // UI界面指针，管理游戏界面的所有控件
    QProgressBar* progressBar = nullptr; // 进度条控件，显示剩余时间
    QTimer* progressTimer = nullptr;     // 进度定时器，控制游戏时间
    Player* player;                      // 玩家对象指针
    QVector<QVector<Block*>> blocks;     // 14x14游戏地图，blocks[y][x]，(2,2)-(11,11)为游戏区，其余为state=0
    int rows = 14, cols = 14;            // 地图行数和列数
    int maxTime = 120;                   // 游戏最大时间（秒）
    int timeLeft = maxTime;              // 剩余时间（秒）
    int formNum = 3;                     // 方块形状种类数量
    int topX = 250, topY = 80;           // 游戏区域左上角坐标
    int blockWidth = 0, blockHeight = 0; // 方块宽度和高度（像素）
    bool timeFlag = true;                // 时间标志，控制时间是否继续计时
    std::array<int, 3> blockTextureIds;  // 本局使用的三个方块贴图编号（1-7）
    QString playerTextureFile;           // 玩家贴图文件名
    std::array<QPixmap, 3> blockPixmaps; // 三个方块贴图
    QPixmap playerPixmap;                // 玩家贴图
    void initTextures();                 // 初始化贴图资源
    Block* activeBlock = nullptr;        // 当前激活的方块
    Block* lastActiveBlock = nullptr;    // 上一次激活的方块
    void handleMove(int dx, int dy);     // 处理玩家移动
    void tryActivateBlock(int bx, int by); // 处理激活方块
    bool canLink(int x1, int y1, int x2, int y2, QVector<QPoint>* path = nullptr); // 判断两方块是否可连
    bool canEliminate(Block* block1, Block* block2); // 判断两方块是否可以消除
    bool canLinkInLine(int x1, int y1, int x2, int y2); // 判断两方块是否可直线连接
    bool canLinkWithOneCorner(int x1, int y1, int x2, int y2, QVector<QPoint>* path = nullptr); // 判断一拐点连接
    bool canLinkWithTwoCorners(int x1, int y1, int x2, int y2, QVector<QPoint>* path = nullptr); // 判断两拐点连接
    QVector<QPoint> linkPath;            // 存储当前需要高亮的消除路径
    void drawLinkPath(QPainter& painter); // 绘制消除路径
    int score = 0;                       // 玩家分数
    QLabel* scoreLabel = nullptr;        // 分数显示控件
    void updateScore(int delta);         // 更新分数
    void updateScoreLabel();             // 刷新分数显示
    void checkGameOver();                // 检查游戏是否结束
    QVector<Item*> props;                // 道具容器
    QTimer* propTimer = nullptr;         // 道具生成定时器
    QTimer* hintTimer = nullptr;         // Hint道具计时器
    QTimer* flashTimer = nullptr;        // Flash道具计时器
    bool flashActive = false;            // Flash道具激活标志
    bool hintActive = false;             // Hint道具激活标志
    QPoint hintBlock1, hintBlock2;       // Hint高亮的两个方块
    void generateProp();                 // 生成道具
    void checkPropCollision();           // 检查玩家与道具碰撞
    void drawProps(QPainter& painter);   // 绘制道具
    void triggerPropEffect(ItemType type); // 触发道具效果
    bool isHintPairValid();              // 检查当前Hint方块对是否有效
    void findHintPair();                 // 查找可消除对用于Hint

protected:
    // 重写绘制事件
    // event: 绘制事件指针
    // 处理窗口绘制，包括方块、玩家、道具、路径等的绘制
    void paintEvent(QPaintEvent* event) override;
    
    // 重写按键事件
    // event: 按键事件指针
    // 处理WASD按键，控制玩家移动
    void keyPressEvent(QKeyEvent* event) override;
    
    // 重写鼠标点击事件
    // event: 鼠标事件指针
    // 支持Flash道具的鼠标点击移动功能
    void mousePressEvent(QMouseEvent* event) override;
    
    PauseMenu* pauseMenu = nullptr;      // 暂停菜单指针
    bool isPaused = false;               // 游戏是否暂停
};

