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
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QRandomGenerator>
#include <queue>
#include <algorithm>
#include <QDebug>
#include <QPainterPath>
#include <array>
#include <cmath>
#include "block.h"
#include "player.h"
#include "ui_duomode.h"
#include "item.h"
#include "load.h"
#include "pausemenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DuoModeClass; }
QT_END_NAMESPACE

// 双人模式游戏窗口类
// 继承自QMainWindow，实现连连看游戏的双人模式
// 包含完整的双人游戏逻辑：双玩家管理、方块管理、路径查找、道具系统、计分系统等
class DuoMode : public QMainWindow
{
    Q_OBJECT
public:
    // 声明测试类为友元类
    friend class SimpleTest;
    // 构造函数
    // parent: 父窗口指针，默认为nullptr
    // saveData: 存档数据指针，用于加载游戏状态，默认为nullptr
    // 初始化双人模式游戏窗口，设置游戏界面和逻辑
    DuoMode(QWidget *parent = nullptr, const SaveData* saveData = nullptr);
    
    // 析构函数
    // 清理游戏资源，停止定时器，删除动态分配的对象
    ~DuoMode();
    
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
    
    // 暂停按钮点击槽函数
    // 处理暂停按钮点击事件
    void on_pauseBtn_clicked();
    
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
    // 返回当前游戏状态的存档数据
    // 收集当前游戏的所有状态信息，用于保存游戏
    SaveData getSaveData() const;
    
    // 应用存档数据
    // data: 要应用的存档数据
    // 将存档数据应用到当前游戏，恢复游戏状态
    void applySaveData(const SaveData& data);

private:
    Ui::DuoModeClass *ui;              // UI界面指针，管理游戏界面的所有控件
    QProgressBar* progressBar = nullptr; // 进度条控件，显示剩余时间
    QTimer* progressTimer = nullptr;     // 进度定时器，控制游戏时间
    Player* player1 = nullptr;           // 玩家1对象指针
    Player* player2 = nullptr;           // 玩家2对象指针
    QVector<QVector<Block*>> blocks;     // 14x14游戏地图，blocks[y][x]，(2,2)-(11,11)为游戏区，其余为state=0
    int rows = 14, cols = 14;            // 地图行数和列数
    int maxTime = 120;                   // 游戏最大时间（秒）
    int timeLeft = maxTime;              // 剩余时间（秒）
    int formNum = 3;                     // 方块形状种类数量
    int topX = 250, topY = 80;           // 游戏区域左上角坐标
    int blockWidth = 50, blockHeight = 50; // 方块宽度和高度（像素）
    bool timeFlag = true;                // 时间标志，控制时间是否继续计时
    std::array<int, 3> blockTextureIds;  // 本局使用的三个方块贴图编号（1-7）
    QString player1TextureFile, player2TextureFile; // 玩家1和玩家2贴图文件名
    std::array<QPixmap, 3> blockPixmaps; // 三个方块贴图
    QPixmap player1Pixmap, player2Pixmap; // 玩家1和玩家2贴图
    void initTextures();                 // 初始化贴图资源
    Block* activeBlock1 = nullptr;       // 玩家1当前激活的方块
    Block* activeBlock2 = nullptr;       // 玩家2当前激活的方块
    void handleMove(int dx, int dy, int playerId); // 处理玩家移动
    void tryActivateBlock(int bx, int by, int playerId); // 处理激活方块
    bool canLink(int x1, int y1, int x2, int y2, QVector<QPoint>* path = nullptr); // 判断两方块是否可连
    bool canEliminate(Block* block1, Block* block2); // 判断两方块是否可以消除
    bool canLinkInLine(int x1, int y1, int x2, int y2); // 判断两方块是否可直线连接
    bool canLinkWithOneCorner(int x1, int y1, int x2, int y2, QVector<QPoint>* path = nullptr); // 判断一拐点连接
    bool canLinkWithTwoCorners(int x1, int y1, int x2, int y2, QVector<QPoint>* path = nullptr); // 判断两拐点连接
    QVector<QPoint> linkPath;            // 存储当前需要高亮的消除路径
    void drawLinkPath(QPainter& painter); // 绘制消除路径
    int score1 = 0, score2 = 0;          // 玩家1和玩家2的分数
    QLabel* score1Label = nullptr;       // 玩家1分数显示控件
    QLabel* score2Label = nullptr;       // 玩家2分数显示控件
    void updateScore(int delta, int playerId); // 更新分数
    void updateScoreLabels();            // 刷新分数显示
    void checkGameOver();                // 检查游戏是否结束
    QVector<Item*> props;                // 道具容器
    QTimer* propTimer = nullptr;         // 道具生成定时器
    QTimer* hintTimer = nullptr;         // Hint道具计时器
    QTimer* freezeTimer1 = nullptr;      // 玩家1冻结计时器
    QTimer* freezeTimer2 = nullptr;      // 玩家2冻结计时器
    QTimer* dizzyTimer1 = nullptr;       // 玩家1眩晕计时器
    QTimer* dizzyTimer2 = nullptr;       // 玩家2眩晕计时器
    bool freezeActive1 = false;          // 玩家1冻结标志
    bool freezeActive2 = false;          // 玩家2冻结标志
    bool dizzyActive1 = false;           // 玩家1眩晕标志
    bool dizzyActive2 = false;           // 玩家2眩晕标志
    bool flashActive1 = false;           // 玩家1 Flash道具激活标志
    bool flashActive2 = false;           // 玩家2 Flash道具激活标志
    void generateProp();                 // 生成道具
    void checkPropCollision(int playerId); // 检查玩家与道具碰撞
    void drawProps(QPainter& painter);   // 绘制道具
    void triggerPropEffect(ItemType type, int playerId); // 触发道具效果
    bool isHintPairValid();              // 检查当前Hint方块对是否有效
    void findHintPair();                 // 查找可消除对用于Hint
    bool hintActive = false;             // Hint道具激活标志
    QPoint hintBlock1, hintBlock2;       // Hint高亮的两个方块

protected:
    // 重写绘制事件
    // event: 绘制事件指针
    // 处理窗口绘制，包括方块、玩家、道具、路径等的绘制
    void paintEvent(QPaintEvent* event) override;
    
    // 重写按键事件
    // event: 按键事件指针
    // 处理WASD按键（玩家1）和方向键（玩家2），控制玩家移动
    void keyPressEvent(QKeyEvent* event) override;
    
    // 重写鼠标点击事件
    // event: 鼠标事件指针
    // 支持Flash道具的鼠标点击移动功能
    void mousePressEvent(QMouseEvent* event) override;
    
    PauseMenu* pauseMenu = nullptr;      // 暂停菜单指针
    bool isPaused = false;               // 游戏是否暂停
};

