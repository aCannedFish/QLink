// load.cpp
// 游戏存档系统实现文件
// 实现连连看游戏的存档和读档功能，包括游戏数据结构和文件操作函数

#include "load.h"
#include <QFile>
#include <QTextStream>

// 保存游戏到文件
// path: 存档文件路径
// data: 要保存的游戏数据
// 返回保存是否成功
// 将游戏状态保存到指定的文本文件中：
// 1. 保存游戏模式、时间、分数
// 2. 保存玩家位置
// 3. 保存方块形状和状态矩阵
// 4. 保存道具信息
bool saveGame(const QString& path, const SaveData& data) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    // 保存游戏模式
    out << (data.mode == GameMode::Single ? "Single" : "Duo") << "\n";
    // 保存剩余时间
    out << data.timeLeft << "\n";
    // 保存玩家分数
    out << data.score1 << " " << data.score2 << "\n";
    // 保存玩家位置
    out << data.player1Pos.x() << " " << data.player1Pos.y() << "\n";
    out << data.player2Pos.x() << " " << data.player2Pos.y() << "\n";
    // 保存地图大小
    out << data.rows << " " << data.cols << "\n";
    // 保存方块贴图ID
    out << data.blockTextureIds[0] << " " << data.blockTextureIds[1] << " " << data.blockTextureIds[2] << "\n";
    // 保存方块形状矩阵
    for (int i = 0; i < data.rows; ++i) {
        for (int j = 0; j < data.cols; ++j)
            out << data.blockForms[i][j] << " ";
        out << "\n";
    }
    // 保存方块状态矩阵
    for (int i = 0; i < data.rows; ++i) {
        for (int j = 0; j < data.cols; ++j)
            out << data.blockStates[i][j] << " ";
        out << "\n";
    }
    // 保存道具信息
    out << data.propPositions.size() << "\n"; // 道具数量
    for (int i = 0; i < data.propPositions.size(); ++i)
        out << data.propPositions[i].x() << " " << data.propPositions[i].y() << " " << data.propTypes[i] << "\n";
    return true;
}

// 从文件加载游戏
// path: 存档文件路径
// data: 用于存储加载的游戏数据
// 返回加载是否成功
// 从指定的文本文件中读取游戏状态，文件格式与saveGame函数对应：
// 按照保存时的顺序依次读取各项数据，并进行数据验证
bool loadGame(const QString& path, SaveData& data) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&file);
    // 读取游戏模式
    QString modeStr; 
    in >> modeStr;
    data.mode = (modeStr == "Single" ? GameMode::Single : GameMode::Duo);
    // 读取剩余时间
    in >> data.timeLeft;
    // 读取玩家分数
    in >> data.score1 >> data.score2;
    // 读取玩家位置
    int x, y;
    in >> x >> y; 
    data.player1Pos = QPoint(x, y);
    in >> x >> y; 
    data.player2Pos = QPoint(x, y);
    // 读取地图大小
    in >> data.rows >> data.cols;
    // 读取方块贴图ID
    in >> data.blockTextureIds[0] >> data.blockTextureIds[1] >> data.blockTextureIds[2];
    // 初始化方块矩阵
    data.blockForms = QVector<QVector<int>>(data.rows, QVector<int>(data.cols));
    data.blockStates = QVector<QVector<int>>(data.rows, QVector<int>(data.cols));
    // 读取方块形状矩阵
    for (int i = 0; i < data.rows; ++i)
        for (int j = 0; j < data.cols; ++j)
            in >> data.blockForms[i][j];
    // 读取方块状态矩阵
    for (int i = 0; i < data.rows; ++i)
        for (int j = 0; j < data.cols; ++j)
            in >> data.blockStates[i][j];
    // 读取道具信息
    int propCount;
    in >> propCount;
    data.propPositions.clear();
    data.propTypes.clear();
    for (int i = 0; i < propCount; ++i) {
        int px, py, ptype;
        in >> px >> py >> ptype;
        data.propPositions.append(QPoint(px, py));
        data.propTypes.append(ptype);
    }
    return true;
}





