#pragma once

#include <QObject>
#include <QTest>
#include "simplemode.h"
#include "duomode.h"

class SimpleTest : public QObject
{
    Q_OBJECT

private slots:
    // 测试单人模式的直线连接判定
    // 测试canLinkInLine函数的各种情况：
    // 1. 水平直线连接（相邻和非相邻）
    // 2. 垂直直线连接（相邻和非相邻）
    // 3. 中间有障碍物的情况
    // 4. 无效的连接情况
    void testSimpleModeCanLinkInLine();
    
    // 测试单人模式的一拐点连接判定
    // 测试canLinkWithOneCorner函数的各种情况：
    // 1. L型连接（拐点1和拐点2）
    // 2. 拐点被占用的情况
    // 3. 拐点到两端路径被阻挡的情况
    void testSimpleModeCanLinkWithOneCorner();
    
    // 测试单人模式的两拐点连接判定
    // 测试canLinkWithTwoCorners函数的各种情况：
    // 1. 水平中间行连接
    // 2. 垂直中间列连接
    // 3. 两个拐点被占用的情况
    // 4. 路径被阻挡的情况
    void testSimpleModeCanLinkWithTwoCorners();
    
    // 测试单人模式的综合连接判定
    // 测试canLink函数，验证三种连接方式的优先级和综合判定
    void testSimpleModeCanLink();
    
    // 测试双人模式的直线连接判定
    // 测试双人模式中canLinkInLine函数的正确性
    void testDuoModeCanLinkInLine();
    
    // 测试双人模式的一拐点连接判定
    // 测试双人模式中canLinkWithOneCorner函数的正确性
    void testDuoModeCanLinkWithOneCorner();
    
    // 测试双人模式的两拐点连接判定
    // 测试双人模式中canLinkWithTwoCorners函数的正确性
    void testDuoModeCanLinkWithTwoCorners();
    
    // 测试双人模式的综合连接判定
    // 测试双人模式中canLink函数的正确性
    void testDuoModeCanLink();

private:
    // 创建测试用的SimpleMode对象
    // 返回配置好的SimpleMode对象指针
    // 创建用于测试的游戏场景，设置特定的方块布局
    SimpleMode* createTestSimpleMode();
    
    // 创建测试用的DuoMode对象
    // 返回配置好的DuoMode对象指针
    // 创建用于测试的双人游戏场景，设置特定的方块布局
    DuoMode* createTestDuoMode();
    
    // 设置测试方块布局
    // mode: 游戏模式对象（SimpleMode或DuoMode）
    // layout: 布局数组，0表示空地，1表示有方块
    // 根据给定的布局数组设置测试场景的方块分布
    template<typename T>
    void setupTestLayout(T* mode, const int layout[14][14]);
};

