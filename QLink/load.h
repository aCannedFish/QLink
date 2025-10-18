// load.h
// 游戏存档系统头文件
// 定义连连看游戏的存档和读档功能，包含游戏数据结构和文件操作函数

#pragma once
#include <QString>
#include <QVector>
#include <QPoint>
#include <array>
#include "item.h"

// 游戏模式枚举
// 定义游戏的两种模式
enum class GameMode { 
    Single, // 单机模式：单人游戏
    Duo     // 双人模式：双人对战
};

// 游戏存档数据结构
// 包含游戏的所有状态信息，用于保存和加载游戏进度
struct SaveData {
    GameMode mode;                    // 游戏模式（单机/双人）
    int timeLeft;                     // 剩余时间（秒）
    int score1, score2;               // 玩家1和玩家2的分数
    QPoint player1Pos, player2Pos;    // 玩家1和玩家2的位置
    int rows = 14;                    // 地图行数
    int cols = 14;                    // 地图列数
    QVector<QPoint> propPositions;    // 道具位置列表
    QVector<int> propTypes;           // 道具类型列表
    QVector<QVector<int>> blockForms; // 方块形状矩阵（14x14）
    QVector<QVector<int>> blockStates;// 方块状态矩阵（14x14）
    std::array<int, 3> blockTextureIds; // 本局使用的三个方块贴图编号
};

// 保存游戏到文件
// path: 存档文件路径
// data: 要保存的游戏数据
// 返回保存是否成功
// 将游戏状态保存到指定的文本文件中：
// 1. 保存游戏模式、时间、分数
// 2. 保存玩家位置
// 3. 保存方块形状和状态矩阵
// 4. 保存道具信息
bool saveGame(const QString& path, const SaveData& data);

// 从文件加载游戏
// path: 存档文件路径
// data: 用于存储加载的游戏数据
// 返回加载是否成功
// 从指定的文本文件中读取游戏状态：
// 1. 读取游戏模式、时间、分数
// 2. 读取玩家位置
// 3. 读取方块形状和状态矩阵
// 4. 读取道具信息
bool loadGame(const QString& path, SaveData& data);