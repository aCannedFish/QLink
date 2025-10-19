#include "simpletest.h"
#include <QTest>
#include <QVector>
#include <QPoint>

SimpleMode* SimpleTest::createTestSimpleMode() {
    return new SimpleMode(nullptr);
}

DuoMode* SimpleTest::createTestDuoMode() {
    return new DuoMode(nullptr);
}

// 设置测试方块布局
template<typename T>
void SimpleTest::setupTestLayout(T* mode, const int layout[14][14]) {
    for (int i = 2; i < 12; ++i) {
        for (int j = 2; j < 12; ++j) {
            if (mode->blocks[i][j]) {
                mode->blocks[i][j]->setState(layout[i][j]);
            }
        }
    }
}

// 测试单人模式的直线连接判定
// 测试canLinkInLine函数的各种情况
void SimpleTest::testSimpleModeCanLinkInLine() {
    SimpleMode* mode = createTestSimpleMode();
    int layout[14][14] = {0};

    // 水平相邻连接
    layout[2][2] = 1;
    layout[2][3] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 3, 2));

    // 垂直相邻连接
    layout[2][2] = 1;
    layout[3][2] = 1;
    layout[2][3] = 0;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 2, 3));

    // 水平非相邻，中间有障碍物
    layout[2][2] = 1;
    layout[2][5] = 1;
    layout[2][3] = 0;
    layout[2][4] = 1; // 障碍
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkInLine(2, 2, 5, 2));

    // 水平非相邻，中间无障碍物
    layout[2][4] = 0;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 5, 2));

    // 垂直非相邻，中间有障碍物
    layout[2][2] = 1;
    layout[5][2] = 1;
    layout[3][2] = 0;
    layout[4][2] = 1; // 障碍
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkInLine(2, 2, 2, 5));

    // 垂直非相邻，中间无障碍物
    layout[4][2] = 0;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 2, 5));

    // 不在同一行或同一列
    layout[2][2] = 1;
    layout[3][3] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkInLine(2, 2, 3, 3));

    delete mode;
}

// 测试单人模式的一拐点连接判定
// 测试canLinkWithOneCorner函数的各种情况
void SimpleTest::testSimpleModeCanLinkWithOneCorner() {
    SimpleMode* mode = createTestSimpleMode();
    QVector<QPoint> path;
    int layout[14][14] = {0};

    // 基础可连
    layout[2][2] = 1;
    layout[4][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    // 拐点1被阻挡，拐点2可用
    layout[2][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    // 两个拐点都被阻挡
    layout[4][2] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    // 拐点到终点路径被阻挡，另一拐点可用
    layout[2][4] = 0;
    layout[4][2] = 0;
    layout[3][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    delete mode;
}

// 测试单人模式的两拐点连接判定
// 测试canLinkWithTwoCorners函数的各种情况
void SimpleTest::testSimpleModeCanLinkWithTwoCorners() {
    SimpleMode* mode = createTestSimpleMode();
    QVector<QPoint> path;
    int layout[14][14] = {0};

    // 基础可连
    layout[2][2] = 1;
    layout[6][6] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithTwoCorners(2, 2, 6, 6, &path));

    // 强制用中间列
    for (int i = 0; i < 14; ++i)
        if (i != 2 && i != 6) {
            layout[i][2] = 1;
            layout[i][6] = 1;
        }
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithTwoCorners(2, 2, 6, 6, &path));

    // 某些拐点被阻挡
    for (int i = 0; i < 14; ++i)
        for (int j = 0; j < 14; ++j)
            layout[i][j] = 0;
    layout[2][2] = 1;
    layout[6][6] = 1;
    layout[4][2] = 1;
    layout[4][6] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithTwoCorners(2, 2, 6, 6, &path));

    // 路径完全被阻挡
    for (int i = 3; i < 12; ++i)
        layout[i][2] = 1;
    for (int j = 3; j < 12; ++j)
        layout[2][j] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkWithTwoCorners(2, 2, 6, 6, &path));

    delete mode;
}

// 测试单人模式的综合连接判定
// 测试canLink函数，验证三种连接方式的优先级和综合判定
void SimpleTest::testSimpleModeCanLink() {
    SimpleMode* mode = createTestSimpleMode();
    QVector<QPoint> path;
    int layout[14][14] = {0};

    // 直线连接
    layout[2][2] = 1;
    layout[2][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLink(2, 2, 4, 2, &path));

    // 一拐点连接
    layout[2][2] = 1;
    layout[4][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLink(2, 2, 4, 4, &path));

    // 两拐点连接
    layout[2][3] = 1;
    layout[2][4] = 1;
    layout[4][2] = 1;
    setupTestLayout(mode, layout);
    layout[2][2] = 1;
    layout[4][4] = 1;
    QVERIFY(mode->canLink(2, 2, 4, 4, &path));

    // 完全无法连接
    for (int i = 1; i < 12; ++i)
        for (int j = 1; j < 12; ++j)
            layout[i][j] = 1;
    layout[2][2] = 1;
    layout[4][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLink(2, 2, 4, 4, &path));

    delete mode;
}

// 测试双人模式的直线连接判定
// 测试双人模式中canLinkInLine函数的正确性
void SimpleTest::testDuoModeCanLinkInLine() {
    DuoMode* mode = createTestDuoMode();
    int layout[14][14] = {0};

    // 水平相邻连接
    layout[2][2] = 1;
    layout[2][3] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 3, 2));

    // 垂直相邻连接
    layout[2][2] = 1;
    layout[3][2] = 1;
    layout[2][3] = 0;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 2, 3));

    // 水平非相邻，中间有障碍物
    layout[2][2] = 1;
    layout[2][5] = 1;
    layout[2][3] = 0;
    layout[2][4] = 1; // 障碍
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkInLine(2, 2, 5, 2));

    // 水平非相邻，中间无障碍物
    layout[2][4] = 0;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkInLine(2, 2, 5, 2));

    // 不在同一行或同一列
    layout[2][2] = 1;
    layout[3][3] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkInLine(2, 2, 3, 3));

    delete mode;
}

// 测试双人模式的一拐点连接判定
// 测试双人模式中canLinkWithOneCorner函数的正确性
void SimpleTest::testDuoModeCanLinkWithOneCorner() {
    DuoMode* mode = createTestDuoMode();
    QVector<QPoint> path;
    int layout[14][14] = {0};

    // 基础可连
    layout[2][2] = 1;
    layout[4][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    // 拐点1被阻挡，拐点2可用
    layout[2][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    // 两个拐点都被阻挡
    layout[4][2] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkWithOneCorner(2, 2, 4, 4, &path));

    delete mode;
}

// 测试双人模式的两拐点连接判定
// 测试双人模式中canLinkWithTwoCorners函数的正确性
void SimpleTest::testDuoModeCanLinkWithTwoCorners() {
    DuoMode* mode = createTestDuoMode();
    QVector<QPoint> path;
    int layout[14][14] = {0};

    // 基础可连
    layout[2][2] = 1;
    layout[6][6] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLinkWithTwoCorners(2, 2, 6, 6, &path));

    // 路径被阻挡
    for (int i = 3; i < 12; ++i)
        layout[i][2] = 1;
    for (int j = 3; j < 12; ++j)
        layout[2][j] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLinkWithTwoCorners(2, 2, 6, 6, &path));

    delete mode;
}

// 测试双人模式的综合连接判定
// 测试双人模式中canLink函数的正确性
void SimpleTest::testDuoModeCanLink() {
    DuoMode* mode = createTestDuoMode();
    QVector<QPoint> path;
    int layout[14][14] = {0};

    // 直线连接
    layout[2][2] = 1;
    layout[2][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLink(2, 2, 4, 2, &path));

    // 一拐点连接
    layout[2][2] = 1;
    layout[4][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(mode->canLink(2, 2, 4, 4, &path));

    // 两拐点连接
    layout[2][3] = 1;
    layout[2][4] = 1;
    layout[4][2] = 1;
    setupTestLayout(mode, layout);
    layout[2][2] = 1;
    layout[4][4] = 1;
    QVERIFY(mode->canLink(2, 2, 4, 4, &path));

    // 完全无法连接
    for (int i = 1; i < 12; ++i)
        for (int j = 1; j < 12; ++j)
            layout[i][j] = 1;
    layout[2][2] = 1;
    layout[4][4] = 1;
    setupTestLayout(mode, layout);
    QVERIFY(!mode->canLink(2, 2, 4, 4, &path));

    delete mode;
}

// QTEST_MAIN(SimpleTest)
