// simplemode.cpp
// 单机模式游戏窗口实现文件
// 实现连连看游戏单机模式的完整功能，包括：
// 
// 主要功能模块：
// 1. 游戏初始化：设置界面、初始化方块、玩家、道具系统
// 2. 游戏逻辑：玩家移动、方块激活、路径查找、消除判断
// 3. 路径查找算法：直线连接、一拐点连接、两拐点连接
// 4. 道具系统：6种道具的生成、碰撞检测、效果触发
// 5. 计分系统：分数计算、显示更新
// 6. 存档系统：游戏状态保存和加载
// 7. 暂停系统：游戏暂停和恢复
// 
// 核心算法：
// - 路径查找算法
// - 方块消除算法：通过形状匹配和路径验证实现方块消除
// - 道具生成算法：随机生成道具并放置在游戏区域
// - 洗牌算法：Fisher-Yates洗牌算法重新排列方块
// 
// 游戏规则：
// - 玩家使用WASD键移动
// - 点击方块激活，相同形状的方块可以消除
// - 消除路径最多只能有两个拐点
// - 游戏时间120秒，超时游戏结束
// - 消除所有方块获得胜利

#include "simplemode.h"
#include <QString>
#include <QPixmap>
#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QRandomGenerator>
#include <queue>
#include <algorithm>
#include <QDebug>
#include <QPainterPath>
#include <QLabel>
#include "item.h"
#include <QTimer>
#include "load.h"
#include "pausemenu.h"
#include <QFileDialog>

// 构造函数
// parent: 父窗口指针，默认为nullptr
// saveData: 存档数据指针，用于加载游戏状态，默认为nullptr
// 初始化单机模式游戏窗口，设置游戏界面和逻辑
SimpleMode::SimpleMode(QWidget *parent, const SaveData* saveData)
    : QMainWindow(parent)
    , ui(new Ui::SimpleModeClass())
{
    blockWidth = 50;
    blockHeight = 50;
    ui->setupUi(this);
    
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(1200, 800);
    this->setStyleSheet("SimpleMode { background-image: url(:/images/images/background2.png); }");
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint); // 禁用右上角关闭

    ui->exitBtn->setStyleSheet("color: rgb(147, 218, 100);");
    ui->pauseBtn->setStyleSheet("color: rgb(147, 218, 100);");
    connect(ui->exitBtn, &QPushButton::clicked, this, &SimpleMode::on_exitBtn_clicked);
    connect(ui->pauseBtn, &QPushButton::clicked, this, &SimpleMode::pauseGame);

    score = 0;
    maxTime = 120;
    timeLeft = maxTime;
    progressBar = new QProgressBar(this);
    progressBar->setGeometry(250, 20, 700, 40);
    progressBar->setRange(0, maxTime);
    progressBar->setValue(maxTime);
    QString timeStr = QString::number(maxTime) + "s";
    progressBar->setFormat(timeStr);
    progressBar->setStyleSheet("QProgressBar{height:22px; text-align:center; font-size:14px; color:white; border-radius:4px; background:rgb(147, 218, 100);}"
                               "QProgressBar::chunk{border-radius:4px;background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 rgb(147, 218, 100),stop:1 rgb(205,218,224));}");
    progressTimer = new QTimer(this);
    connect(progressTimer, &QTimer::timeout, this, &SimpleMode::progress);
    progressTimer->start(1000);
    // 道具定时器
    propTimer = new QTimer(this);
    connect(propTimer, &QTimer::timeout, this, &SimpleMode::generateProp);
    propTimer->start(30000); // 30秒
    // Hint/Flash定时器初始化
    hintTimer = new QTimer(this);
    flashTimer = new QTimer(this);
    connect(hintTimer, &QTimer::timeout, this, [this]() {
        hintActive = false;
        hintBlock1 = QPoint(-1, -1);
        hintBlock2 = QPoint(-1, -1);
        hintTimer->stop();
        update();
    });
    connect(flashTimer, &QTimer::timeout, this, [this]() {
        flashActive = false;
        flashTimer->stop();
        update();
    });
    // blocks初始化为rows*cols，边界为state=0，游戏区后面填充
    blocks.resize(rows);
    for (int i = 0; i < rows; ++i) {
        blocks[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            if (i >= 2 && i < rows-2 && j >= 2 && j < cols-2) {
                blocks[i][j] = nullptr; // 游戏区后面填充
            } else {
                blocks[i][j] = new Block(topX + j * blockWidth, topY + i * blockHeight, blockWidth, blockHeight, 0, 0);
                blocks[i][j]->setMapXY(j, i);
            }
        }
    }
    // 生成有效方块区
    QVector<Block*> gameBlocks;
    for (int i = 2; i < rows-2; ++i) {
        for (int j = 2; j < cols-2; j += 2) {
            int randomForm = QRandomGenerator::global()->bounded(formNum);
            Block* b1 = new Block(topX + j * blockWidth, topY + i * blockHeight, blockWidth, blockHeight, randomForm, 1);
            Block* b2 = new Block(topX + (j + 1) * blockWidth, topY + i * blockHeight, blockWidth, blockHeight, randomForm, 1);
            b1->setMapXY(j, i);
            b2->setMapXY(j + 1, i);
            gameBlocks.append(b1);
            gameBlocks.append(b2);
        }
    }
    int idx = 0;
    for (int i = 2; i < rows-2; ++i) {
        for (int j = 2; j < cols-2; ++j) {
            blocks[i][j] = gameBlocks[idx++];
        }
    }
    shuffle();
    initTextures();
    this->setFocusPolicy(Qt::StrongFocus);
    player = new Player(topX, topY, 0);
    // 设置玩家初始地图坐标（地图左上角）
    player->setXInMap(0);
    player->setYInMap(0);

    // 分数显示控件
    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(20, 720, 200, 60);
    scoreLabel->setStyleSheet("font-size:40px;color:rgb(102,178,255);font-weight:bold;font-style:italic;");
    updateScoreLabel();

    // 游戏开始时立即生成一个道具
    generateProp();
    
    // 如果有存档数据，应用存档
    if (saveData) applySaveData(*saveData);
}

// 析构函数
// 清理游戏资源，停止定时器，删除动态分配的对象
SimpleMode::~SimpleMode()
{
    for (Item* prop : props) delete prop;
    delete ui;
}

// 更新分数
// delta: 分数增量
void SimpleMode::updateScore(int delta) {
    score += delta;
    updateScoreLabel();
}

// 更新分数显示
void SimpleMode::updateScoreLabel() {
    scoreLabel->setText(QString("分数: %1").arg(score));
}

// 检查游戏是否结束
// 如果没有可消除的方块对，游戏结束
void SimpleMode::checkGameOver() {
    for (int i = 2; i < rows-2; ++i) {
        for (int j = 2; j < cols-2; ++j) {
            Block* b1 = blocks[i][j];
            if (!b1 || b1->getState() == 0) continue;
            for (int ii = 2; ii < rows-2; ++ii) {
                for (int jj = 2; jj < cols-2; ++jj) {
                    if (i == ii && j == jj) continue;
                    Block* b2 = blocks[ii][jj];
                    if (!b2 || b2->getState() == 0) continue;
                    if (b1->getForm() == b2->getForm() && canLink(b1->getMapX(), b1->getMapY(), b2->getMapX(), b2->getMapY())) {
                        return; // 还有可消除对
                    }
                }
            }
        }
    }
    // 没有可消除对，游戏结束
    progressTimer->stop(); // 停止计时器，防止时间到时再次弹出弹窗
    QMessageBox::information(this, "游戏结束", QString("游戏结束！最终分数：%1").arg(score));
    close();
}

// 游戏进度更新
// 更新游戏状态，包括时间、分数等
void SimpleMode::progress()
{
    if (timeLeft > 0) {
        timeLeft--;
        progressBar->setValue(timeLeft);
        QString timeStr = QString::number(timeLeft) + "s";
        progressBar->setFormat(timeStr);
    }
    if (timeLeft == 0) {
        progressTimer->stop();
        QMessageBox::information(this, "游戏结束", QString("时间到！最终分数：%1").arg(score));
        close();
    }
}

// 洗牌功能
// 重新排列所有方块，打乱游戏布局
void SimpleMode::shuffle()
{
    QVector<Block*> nonEmptyBlocks;
    QVector<QPair<int, int>> positions;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (blocks[i][j] && blocks[i][j]->getState() != 0) {
                nonEmptyBlocks.append(blocks[i][j]);
                positions.append(qMakePair(i, j));
            }
        }
    }
    std::shuffle(nonEmptyBlocks.begin(), nonEmptyBlocks.end(), *QRandomGenerator::global());
    for (int idx = 0; idx < positions.size(); ++idx) {
        int i = positions[idx].first;
        int j = positions[idx].second;
        blocks[i][j] = nonEmptyBlocks[idx];
        // 同步mapXY和像素坐标，防止消除判定错乱
        blocks[i][j]->setMapXY(j, i);
        blocks[i][j]->setCord(topX + j * blockWidth, topY + i * blockHeight);
    }
}

// 初始化贴图
// 随机选取三个不同的方块贴图编号（1-7）
void SimpleMode::initTextures()
{
    QVector<int> allIds{1,2,3,4,5,6,7};
    std::shuffle(allIds.begin(), allIds.end(), *QRandomGenerator::global());
    for (int i = 0; i < 3; ++i) {
        blockTextureIds[i] = allIds[i];
        QString file = QString(":/images/images/%1-1.png").arg(blockTextureIds[i]);
        blockPixmaps[i] = QPixmap(file).scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    playerTextureFile = ":/images/images/player1.png";
    playerPixmap = QPixmap(playerTextureFile).scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

// 绘制消除路径
// painter: QPainter对象
void SimpleMode::drawLinkPath(QPainter& painter)
{
    if (linkPath.size() < 2) return;
    QPen pen(QColor(160, 160, 160), 3);
    painter.setPen(pen);
    QVector<QPoint> pixelPoints;
    for (const QPoint& pt : linkPath) {
        int px = topX + pt.x() * blockWidth + blockWidth / 2;
        int py = topY + pt.y() * blockHeight + blockHeight / 2;
        pixelPoints.append(QPoint(px, py));
    }
    painter.drawPolyline(pixelPoints.data(), pixelPoints.size());
}

// 生成道具
// 道具生成间隔为30秒，由propTimer控制
void SimpleMode::generateProp() {
    QVector<QPoint> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (blocks[i][j] && blocks[i][j]->getState() == 0) {
                bool occupied = false;
                for (Item* prop : props)
                    if (prop->isVisible() && prop->getMapPos() == QPoint(j, i)) { occupied = true; break; }
                if (!occupied) empty.append(QPoint(j, i));
            }
    if (empty.isEmpty()) return;
    QPoint pos = empty[QRandomGenerator::global()->bounded(empty.size())];
    QRectF rect(topX + pos.x() * blockWidth, topY + pos.y() * blockHeight, blockWidth, blockHeight);
    ItemType type = static_cast<ItemType>(QRandomGenerator::global()->bounded(0, 4));
    QPixmap pix;
    switch (type) {
        case ItemType::AddTime: pix = QPixmap(":/images/images/Time.png"); break;
        case ItemType::Shuffle: pix = QPixmap(":/images/images/Shuffle.png"); break;
        case ItemType::Hint:    pix = QPixmap(":/images/images/Hint.png"); break;
        case ItemType::Flash:   pix = QPixmap(":/images/images/Flash.png"); break;
    }
    Item* prop = new Item(type, pos, rect, pix);
    props.append(prop);
    qDebug() << "生成道具: 类型=" << static_cast<int>(type) << "位置=" << pos;
    update();
}

// 绘制道具
// painter: QPainter对象
void SimpleMode::drawProps(QPainter& painter) {
    for (Item* prop : props)
        if (prop->isVisible())
            prop->draw(painter);
}

// 绘制事件
// event: 绘制事件对象
void SimpleMode::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setBrush(QColor(147, 218, 100));
    painter.setPen(Qt::NoPen);
    painter.drawRect(topX, topY, 700, 700);
    
    if (hintActive && hintBlock1 != QPoint(-1, -1) && hintBlock2 != QPoint(-1, -1)) {
        painter.setPen(QPen(Qt::red, 3));
        painter.setBrush(QColor(255, 0, 0, 80));
        QRectF r1(topX + hintBlock1.x() * blockWidth, topY + hintBlock1.y() * blockHeight, blockWidth, blockHeight);
        QRectF r2(topX + hintBlock2.x() * blockWidth, topY + hintBlock2.y() * blockHeight, blockWidth, blockHeight);
        painter.drawRect(r1);
        painter.drawRect(r2);
    }
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Block* blk = blocks[i][j];
            if (!blk) continue;
            int state = blk->getState();
            if (state == 0) continue;
            int form = blk->getForm();
            if (form < 0 || form >= 3) continue;
            int texNum = blockTextureIds[form];
            QString file = QString(":/images/images/%1-%2.png").arg(texNum).arg(state);
            QPixmap pix(file);
            if (pix.isNull()) pix = blockPixmaps[form];
            painter.drawPixmap(topX + j * blockWidth + 2, topY + i * blockHeight + 2, 46, 46, pix);
        }
    }
    drawProps(painter);
    QRectF playerRect = player->getCord();
    painter.drawPixmap(playerRect.x() + 2, playerRect.y() + 2, 46, 46, playerPixmap);
    drawLinkPath(painter);
}

// 键盘按键事件
// event: 键盘事件对象
void SimpleMode::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) handleMove(0, -1);
    else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) handleMove(0, 1);
    else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) handleMove(-1, 0);
    else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) handleMove(1, 0);
}

// 玩家移动后检测道具
// dx: x方向移动量
// dy: y方向移动量
void SimpleMode::handleMove(int dx, int dy) {
    linkPath.clear();
    int nx = player->getXInMap() + dx;
    int ny = player->getYInMap() + dy;
    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) return;
    if (blocks[ny][nx] && blocks[ny][nx]->getState() != 0) {
        tryActivateBlock(nx, ny);
        return;
    }
    player->setXInMap(nx);
    player->setYInMap(ny);
    player->getCord().moveTo(topX + nx * blockWidth, topY + ny * blockHeight);
    qDebug() << "玩家移动到: 地图坐标(" << nx << "," << ny << ") 像素坐标(" << player->getCord().x() << "," << player->getCord().y() << ")";
    checkPropCollision();
    update();
}

// 激活方块逻辑
// bx: 方块x坐标
// by: 方块y坐标
void SimpleMode::tryActivateBlock(int bx, int by)
{
    linkPath.clear();
    Block* blk = blocks[by][bx];
    if (!player->getActive()) {
        blk->setState(2);
        activeBlock = blk;
        player->setActive(true);
    } else {
        if (blk == activeBlock) {
            blk->setState(1);
            activeBlock = nullptr;
            player->setActive(false);
        } else {
            if (canEliminate(activeBlock, blk)) {
                player->setActive(false);
                activeBlock = nullptr;
            } else {
                activeBlock->setState(1);
                blk->setState(1);
                activeBlock = nullptr;
                player->setActive(false);
            }
        }
    }
    update();
}

// 判断两方块是否可以消除
// block1: 第一个方块指针
// block2: 第二个方块指针
// 返回值: 是否可以消除
bool SimpleMode::canEliminate(Block* block1, Block* block2) {
    if (!block1 || !block2) return false;
    int x1 = block1->getMapX();
    int y1 = block1->getMapY();
    int x2 = block2->getMapX();
    int y2 = block2->getMapY();
    qDebug() << "canEliminate: block1 mapXY(" << x1 << "," << y1 << ") block2 mapXY(" << x2 << "," << y2 << ")";
    if (block1->getForm() == block2->getForm() && canLink(x1, y1, x2, y2, &linkPath)) {
        block1->setState(0);
        block2->setState(0);
        updateScore(2);
        if (hintActive) {
            if (!isHintPairValid()) {
                qDebug() << "当前Hint方块对被消除，寻找下一对";
                findHintPair();
            }
        }
        update();
        checkGameOver();
        return true;
    }
    return false;
}

// 判断两方块是否可以通过直线连接
// x1: 第一个方块的x坐标
// y1: 第一个方块的y坐标
// x2: 第二个方块的x坐标
// y2: 第二个方块的y坐标
// 返回值: 是否可以通过直线连接
bool SimpleMode::canLinkInLine(int x1, int y1, int x2, int y2)
{
    if (x1 == x2) {
		if (std::abs(y1 - y2) == 1) return true;
        for (int y = std::min(y1, y2) + 1; y < std::max(y1, y2); ++y) {
            if (blocks[y][x1]->getState() != 0) {
                return false;
            }
		}
		return true;
    } else if (y1 == y2) {
		if (std::abs(x1 - x2) == 1) return true;
        for (int x = std::min(x1, x2) + 1; x < std::max(x1, x2); ++x) {
            if (blocks[y1][x]->getState() != 0) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// 判断两方块是否可以通过一个拐点连接
// x1: 第一个方块的x坐标
// y1: 第一个方块的y坐标
// x2: 第二个方块的x坐标
// y2: 第二个方块的y坐标
// path: 用于存储连接路径的指针，可以为nullptr
// 返回值: 是否可以通过一个拐点连接
bool SimpleMode::canLinkWithOneCorner(int x1, int y1, int x2, int y2, QVector<QPoint>* path) {
    if (blocks[y2][x1]->getState() == 0 &&
        canLinkInLine(x1, y1, x1, y2) &&
        canLinkInLine(x1, y2, x2, y2)) {
        if (path) {
            path->clear();
            path->append(QPoint(x1, y1));
            path->append(QPoint(x1, y2));
            path->append(QPoint(x2, y2));
        }
        return true;
    }
    if (blocks[y1][x2]->getState() == 0 &&
        canLinkInLine(x1, y1, x2, y1) &&
        canLinkInLine(x2, y1, x2, y2)) {
        if (path) {
            path->clear();
            path->append(QPoint(x1, y1));
            path->append(QPoint(x2, y1));
            path->append(QPoint(x2, y2));
        }
        return true;
    }
    return false;
}

// 判断两方块是否可以通过两个拐点连接
// x1: 第一个方块的x坐标
// y1: 第一个方块的y坐标
// x2: 第二个方块的x坐标
// y2: 第二个方块的y坐标
// path: 用于存储连接路径的指针，可以为nullptr
// 返回值: 是否可以通过两个拐点连接
bool SimpleMode::canLinkWithTwoCorners(int x1, int y1, int x2, int y2, QVector<QPoint>* path) {
    for (int i = 0; i < rows; ++i) {
        if (i == y1 || i == y2) continue;
        if (blocks[i][x1]->getState() == 0 && blocks[i][x2]->getState() == 0 &&
            canLinkInLine(x1, y1, x1, i) &&
            canLinkInLine(x1, i, x2, i) &&
            canLinkInLine(x2, i, x2, y2)) {
            if (path) {
                path->clear();
                path->append(QPoint(x1, y1));
                path->append(QPoint(x1, i));
                path->append(QPoint(x2, i));
                path->append(QPoint(x2, y2));
            }
            return true;
        }
    }
    for (int j = 0; j < cols; ++j) {
        if (j == x1 || j == x2) continue;
        if (blocks[y1][j]->getState() == 0 && blocks[y2][j]->getState() == 0 &&
            canLinkInLine(x1, y1, j, y1) &&
            canLinkInLine(j, y1, j, y2) &&
            canLinkInLine(j, y2, x2, y2)) {
            if (path) {
                path->clear();
                path->append(QPoint(x1, y1));
                path->append(QPoint(j, y1));
                path->append(QPoint(j, y2));
                path->append(QPoint(x2, y2));
            }
            return true;
        }
    }
    return false;
}

// 判断两方块是否可以连接
// x1: 第一个方块的x坐标
// y1: 第一个方块的y坐标
// x2: 第二个方块的x坐标
// y2: 第二个方块的y坐标
// path: 用于存储连接路径的指针，可以为nullptr
// 返回值: 是否可以连接
bool SimpleMode::canLink(int x1, int y1, int x2, int y2, QVector<QPoint>* path) {
    if (canLinkInLine(x1, y1, x2, y2)) {
        if (path) {
            path->clear();
            path->append(QPoint(x1, y1));
            path->append(QPoint(x2, y2));
        }
        return true;
    }
    if (canLinkWithOneCorner(x1, y1, x2, y2, path)) {
        return true;
    }
    if (canLinkWithTwoCorners(x1, y1, x2, y2, path)) {
        return true;
    }
    return false;
}

// 退出按钮点击槽函数
// 处理退出按钮点击事件
void SimpleMode::on_exitBtn_clicked()
{
    progressTimer->stop();
    emit exitToMenu();
    this->close();
}

// 暂停游戏
// 暂停游戏并显示暂停菜单
void SimpleMode::pauseGame() {
    if (isPaused) return;
    isPaused = true;
    if (progressTimer) progressTimer->stop();
    if (propTimer) propTimer->stop();
    if (hintTimer) hintTimer->stop();
    if (flashTimer) flashTimer->stop();
    setEnabled(false);
    pauseMenu = new PauseMenu(this);
    connect(pauseMenu, &PauseMenu::continueClicked, this, &SimpleMode::onContinueBtnClicked);
    connect(pauseMenu, &PauseMenu::exitClicked, this, &SimpleMode::onExitBtnClicked);
    connect(pauseMenu, &PauseMenu::saveClicked, this, &SimpleMode::onSaveBtnClicked);
    connect(pauseMenu, &PauseMenu::loadClicked, this, &SimpleMode::onLoadBtnClicked);
    pauseMenu->exec();
}

// 恢复游戏
// 恢复游戏状态，关闭暂停菜单
void SimpleMode::resumeGame() {
    isPaused = false;
    if (progressTimer) progressTimer->start();
    if (propTimer) propTimer->start();
    if (hintTimer && hintActive) hintTimer->start();
    if (flashTimer && flashActive) flashTimer->start();
    setEnabled(true);
    if (pauseMenu) pauseMenu->close();
}

// 暂停菜单继续按钮点击槽函数
void SimpleMode::onContinueBtnClicked() { resumeGame(); }

// 暂停菜单退出按钮点击槽函数
void SimpleMode::onExitBtnClicked() { if (pauseMenu) pauseMenu->close(); close(); }

// 暂停菜单保存按钮点击槽函数
void SimpleMode::onSaveBtnClicked() {
    QString path = QFileDialog::getSaveFileName(this, "保存存档", "", "存档文件 (*.txt)");
    if (!path.isEmpty()) saveGame(path, getSaveData());
}

// 暂停菜单加载按钮点击槽函数
void SimpleMode::onLoadBtnClicked() {
    QString path = QFileDialog::getOpenFileName(this, "读取存档", "", "存档文件 (*.txt)");
    if (!path.isEmpty()) {
        SaveData data;
        if (loadGame(path, data) && data.mode == GameMode::Single) {
            applySaveData(data);
            resumeGame();
            if (pauseMenu) pauseMenu->close();
        } else {
            QMessageBox::warning(this, "错误", "存档无效或不是单人模式存档！");
        }
    }
}

// 获取存档数据
// 返回值: 存档数据对象
SaveData SimpleMode::getSaveData() const {
    SaveData data;
    data.mode = GameMode::Single;
    data.timeLeft = timeLeft;
    data.score1 = score;
    data.score2 = 0;
    data.player1Pos = QPoint(player->getXInMap(), player->getYInMap());
    data.player2Pos = QPoint(-1, -1);
    data.blockTextureIds = blockTextureIds;
    data.blockForms = QVector<QVector<int>>(rows, QVector<int>(cols));
    data.blockStates = QVector<QVector<int>>(rows, QVector<int>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            data.blockForms[i][j] = blocks[i][j] ? blocks[i][j]->getForm() : -1;
            data.blockStates[i][j] = blocks[i][j] ? blocks[i][j]->getState() : 0;
        }
    data.propPositions.clear();
    data.propTypes.clear();
    for (Item* prop : props) {
        if (prop->isVisible()) {
            data.propPositions.append(prop->getMapPos());
            data.propTypes.append(static_cast<int>(prop->getType()));
        }
    }
    return data;
}

// 应用存档数据
// data: 存档数据对象
void SimpleMode::applySaveData(const SaveData& data) {
    timeLeft = data.timeLeft;
    score = data.score1;
    player->setXInMap(data.player1Pos.x());
    player->setYInMap(data.player1Pos.y());
    player->getCord().moveTo(topX + data.player1Pos.x() * blockWidth, topY + data.player1Pos.y() * blockHeight);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            if (blocks[i][j]) {
                blocks[i][j]->setForm(data.blockForms[i][j]);
                blocks[i][j]->setState(data.blockStates[i][j]);
            }
        }
    for (Item* prop : props) delete prop;
    props.clear();
    for (int i = 0; i < data.propPositions.size(); ++i) {
        QRectF rect(topX + data.propPositions[i].x() * blockWidth, topY + data.propPositions[i].y() * blockHeight, blockWidth, blockHeight);
        QPixmap pix;
        ItemType type = static_cast<ItemType>(data.propTypes[i]);
        switch (type) {
            case ItemType::AddTime: pix = QPixmap(":/images/images/Time.png"); break;
            case ItemType::Shuffle: pix = QPixmap(":/images/images/Shuffle.png"); break;
            case ItemType::Hint:    pix = QPixmap(":/images/images/Hint.png"); break;
            case ItemType::Flash:   pix = QPixmap(":/images/images/Flash.png"); break;
            default: break;
        }
        Item* prop = new Item(type, data.propPositions[i], rect, pix);
        props.append(prop);
    }
    updateScoreLabel();
    update();
}

// 检查道具碰撞
void SimpleMode::checkPropCollision() {
    QPoint playerPos(player->getXInMap(), player->getYInMap());
    for (Item* prop : props) {
        if (prop->isVisible()) {
            if (prop->getMapPos() == playerPos) {
                triggerPropEffect(prop->getType());
                prop->setVisible(false);
            }
        }
    }
}

// 触发道具效果
// type: 道具类型
void SimpleMode::triggerPropEffect(ItemType type) {
    switch (type) {
        case ItemType::AddTime:
            timeLeft += 30;
            if (timeLeft > maxTime) timeLeft = maxTime;
            progressBar->setValue(timeLeft);
            break;
        case ItemType::Shuffle:
            shuffle();
            break;
        case ItemType::Hint:
            hintActive = true;
            findHintPair();
            hintTimer->start(10000);
            break;
        case ItemType::Flash:
            flashActive = true;
            flashTimer->start(5000);
            break;
        default: break;
    }
    update();
}

// 检查当前高亮的Hint方块对是否仍然有效
// 返回值: 如果Hint方块对仍然可以消除则返回true，否则返回false
bool SimpleMode::isHintPairValid() {
    if (hintBlock1 == QPoint(-1, -1) || hintBlock2 == QPoint(-1, -1)) return false;
    Block* b1 = blocks[hintBlock1.y()][hintBlock1.x()];
    Block* b2 = blocks[hintBlock2.y()][hintBlock2.x()];
    if (!b1 || !b2 || b1->getState() == 0 || b2->getState() == 0) return false;
    return b1->getForm() == b2->getForm() && canLink(b1->getMapX(), b1->getMapY(), b2->getMapX(), b2->getMapY());
}

// 查找可消除的方块对用于Hint提示
void SimpleMode::findHintPair() {
    for (int i = 2; i < rows-2; ++i) {
        for (int j = 2; j < cols-2; ++j) {
            Block* b1 = blocks[i][j];
            if (!b1 || b1->getState() == 0) continue;
            for (int ii = 2; ii < rows-2; ++ii) {
                for (int jj = 2; jj < cols-2; ++jj) {
                    if (i == ii && j == jj) continue;
                    Block* b2 = blocks[ii][jj];
                    if (!b2 || b2->getState() == 0) continue;
                    if (b1->getForm() == b2->getForm() && canLink(b1->getMapX(), b1->getMapY(), b2->getMapX(), b2->getMapY())) {
                        hintBlock1 = QPoint(j, i);
                        hintBlock2 = QPoint(jj, ii);
                        return;
                    }
                }
            }
        }
    }
    hintBlock1 = QPoint(-1, -1);
    hintBlock2 = QPoint(-1, -1);
}

// 鼠标点击事件处理
// event: 鼠标事件指针
void SimpleMode::mousePressEvent(QMouseEvent* event) {
    if (!flashActive) return;
    QPoint pos = event->pos();
    int mx = (pos.x() - topX) / blockWidth;
    int my = (pos.y() - topY) / blockHeight;
    if (mx < 0 || mx >= rows || my < 0 || my >= cols) return;
    if (!blocks[my][mx] || blocks[my][mx]->getState() == 0) {
        player->setXInMap(mx);
        player->setYInMap(my);
        player->getCord().moveTo(topX + mx * blockWidth, topY + my * blockHeight);
        checkPropCollision();
        update();
    } else {
        static const int dx[4] = {0, 0, -1, 1};
        static const int dy[4] = {-1, 1, 0, 0};
        for (int d = 0; d < 4; ++d) {
            int nx = mx + dx[d], ny = my + dy[d];
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && (!blocks[ny][nx] || blocks[ny][nx]->getState() == 0)) {
                player->setXInMap(nx);
                player->setYInMap(ny);
                player->getCord().moveTo(topX + nx * blockWidth, topY + ny * blockHeight);
                tryActivateBlock(mx, my);
                checkPropCollision();
                update();
                break;
            }
        }
    }
}










