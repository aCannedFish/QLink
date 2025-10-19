#include "duomode.h"
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
#include <QMessageBox>

// 构造函数
// saveData: 存档数据指针，用于加载游戏状态，默认为nullptr
// 初始化双人模式游戏窗口，设置游戏界面和逻辑
DuoMode::DuoMode(QWidget *parent, const SaveData* saveData)
    : QMainWindow(parent)
    , ui(new Ui::DuoModeClass())
{
    blockWidth = 50;
    blockHeight = 50;
    ui->setupUi(this);
    
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(1200, 800);
    this->setStyleSheet("DuoMode { background-image: url(:/images/images/background2.png); }");
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint); // 禁用右上角关闭

    ui->exitBtn->setStyleSheet("color: rgb(147, 218, 100);");
    ui->pauseBtn->setStyleSheet("color: rgb(147, 218, 100);");
    connect(ui->exitBtn, &QPushButton::clicked, this, &DuoMode::on_exitBtn_clicked);
    connect(ui->pauseBtn, &QPushButton::clicked, this, &DuoMode::pauseGame);

    score1 = 0;
    score2 = 0;
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
    connect(progressTimer, &QTimer::timeout, this, &DuoMode::progress);
    progressTimer->start(1000);
    
    // 道具定时器
    propTimer = new QTimer(this);
    connect(propTimer, &QTimer::timeout, this, &DuoMode::generateProp);
    propTimer->start(30000); // 30秒
    
    // 各种道具定时器初始化
    hintTimer = new QTimer(this);
    freezeTimer1 = new QTimer(this);
    freezeTimer2 = new QTimer(this);
    dizzyTimer1 = new QTimer(this);
    dizzyTimer2 = new QTimer(this);
    
    connect(hintTimer, &QTimer::timeout, this, [this]() {
        hintActive = false;
        hintBlock1 = QPoint(-1, -1);
        hintBlock2 = QPoint(-1, -1);
        hintTimer->stop();
        update();
    });
    
    connect(freezeTimer1, &QTimer::timeout, this, [this]() {
        freezeActive1 = false;
        freezeTimer1->stop();
        update();
    });
    
    connect(freezeTimer2, &QTimer::timeout, this, [this]() {
        freezeActive2 = false;
        freezeTimer2->stop();
        update();
    });
    
    connect(dizzyTimer1, &QTimer::timeout, this, [this]() {
        dizzyActive1 = false;
        dizzyTimer1->stop();
        update();
    });
    
    connect(dizzyTimer2, &QTimer::timeout, this, [this]() {
        dizzyActive2 = false;
        dizzyTimer2->stop();
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
    
    // 创建两个玩家
    player1 = new Player(topX, topY, 0);
    player2 = new Player(topX + (cols - 1) * blockWidth, topY + (rows - 1) * blockHeight, 0); // 玩家2在右下角
    
    // 设置玩家初始地图坐标
    player1->setXInMap(0);
    player1->setYInMap(0);
    player2->setXInMap(cols - 1);
    player2->setYInMap(rows - 1);
    
    // 分数显示控件
    score1Label = new QLabel(this);
    score1Label->setGeometry(20, 720, 200, 60);
    score1Label->setStyleSheet("font-size:40px;color:rgb(102,178,255);font-weight:bold;font-style:italic;");
    
    score2Label = new QLabel(this);
    score2Label->setGeometry(1020, 720, 200, 60);
    score2Label->setStyleSheet("font-size:40px;color:rgb(67,174,74);font-weight:bold;font-style:italic;");
    
    updateScoreLabels();

    // 游戏开始时立即生成一个道具（确保不在玩家起始位置）
    generateProp();
    
    // 如果有存档数据，应用存档
    if (saveData) applySaveData(*saveData);
}

// 析构函数
// 清理游戏资源，停止定时器，删除动态分配的对象
DuoMode::~DuoMode()
{
    for (Item* prop : props) delete prop;
    delete ui;
}

// 更新分数
// delta: 分数变化值
// playerId: 玩家ID（1或2）
void DuoMode::updateScore(int delta, int playerId) {
    if (playerId == 1) {
        score1 += delta;
    } else {
        score2 += delta;
    }
    updateScoreLabels();
}

// 更新分数显示
void DuoMode::updateScoreLabels() {
    score1Label->setText(QString("P1: %1").arg(score1));
    score2Label->setText(QString("P2: %1").arg(score2));
}

// 检查游戏是否结束
// 如果没有可消除对，游戏结束并判定胜负
void DuoMode::checkGameOver() {
    for (int i = 2; i < rows - 2; ++i) {
        for (int j = 2; j < cols - 2; ++j) {
            Block* b1 = blocks[i][j];
            if (!b1 || b1->getState() == 0) continue;
            for (int ii = 2; ii < rows - 2; ++ii) {
                for (int jj = 2; jj < cols - 2; ++jj) {
                    if (i == ii && j == jj) continue;
                    Block* b2 = blocks[ii][jj];
                    if (!b2 || b2->getState() == 0) continue;
                    if (b1->getForm() == b2->getForm() && canLink(b1->getMapX(), b1->getMapY(), b2->getMapX(), b2->getMapY())) {
                        return;
                    }
                }
            }
        }
    }
    progressTimer->stop();
    QString result;
    if (score1 > score2) {
        result = QString("游戏结束！玩家1获胜！\n玩家1: %1分  玩家2: %2分").arg(score1).arg(score2);
    } else if (score2 > score1) {
        result = QString("游戏结束！玩家2获胜！\n玩家1: %1分  玩家2: %2分").arg(score1).arg(score2);
    } else {
        result = QString("游戏结束！平局！\n玩家1: %1分  玩家2: %2分").arg(score1).arg(score2);
    }
    QMessageBox::information(this, "游戏结束", result);
    close();
}

// 游戏进度更新
// 更新游戏状态，包括时间、分数等
void DuoMode::progress()
{
    if (timeLeft > 0) {
        timeLeft--;
        progressBar->setValue(timeLeft);
        QString timeStr = QString::number(timeLeft) + "s";
        progressBar->setFormat(timeStr);
    }
    if (timeLeft == 0) {
        progressTimer->stop();
        QString result;
        if (score1 > score2) {
            result = QString("时间到！玩家1获胜！\n玩家1: %1分  玩家2: %2分").arg(score1).arg(score2);
        } else if (score2 > score1) {
            result = QString("时间到！玩家2获胜！\n玩家1: %1分  玩家2: %2分").arg(score1).arg(score2);
        } else {
            result = QString("时间到！平局！\n玩家1: %1分  玩家2: %2分").arg(score1).arg(score2);
        }
        QMessageBox::information(this, "游戏结束", result);
        close();
    }
}

// 洗牌功能
// 重新排列所有方块，打乱游戏布局
void DuoMode::shuffle()
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
        blocks[i][j]->setMapXY(j, i);
        blocks[i][j]->setCord(topX + j * blockWidth, topY + i * blockHeight);
    }
}

// 初始化方块贴图
void DuoMode::initTextures()
{
    QVector<int> allIds{1,2,3,4,5,6,7};
    std::shuffle(allIds.begin(), allIds.end(), *QRandomGenerator::global());
    for (int i = 0; i < 3; ++i) {
        blockTextureIds[i] = allIds[i];
        QString file = QString(":/images/images/%1-1.png").arg(blockTextureIds[i]);
        blockPixmaps[i] = QPixmap(file).scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    player1TextureFile = ":/images/images/player1.png";
    player2TextureFile = ":/images/images/player2.png";
    player1Pixmap = QPixmap(player1TextureFile).scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    player2Pixmap = QPixmap(player2TextureFile).scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

// 绘制消除路径
void DuoMode::drawLinkPath(QPainter& painter)
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

// 生成道具（双人模式版本）
void DuoMode::generateProp() {
    QVector<QPoint> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (blocks[i][j] && blocks[i][j]->getState() == 0) {
                bool isPlayerPosition = false;
                if (player1 && j == player1->getXInMap() && i == player1->getYInMap()) {
                    isPlayerPosition = true;
                }
                if (player2 && j == player2->getXInMap() && i == player2->getYInMap()) {
                    isPlayerPosition = true;
                }
                if (isPlayerPosition) continue;
                
                bool occupied = false;
                for (Item* prop : props)
                    if (prop->isVisible() && prop->getMapPos() == QPoint(j, i)) { occupied = true; break; }
                if (!occupied) empty.append(QPoint(j, i));
            }
    if (empty.isEmpty()) return;
    QPoint pos = empty[QRandomGenerator::global()->bounded(empty.size())];
    QRectF rect(topX + pos.x() * blockWidth, topY + pos.y() * blockHeight, blockWidth, blockHeight);
    
    QVector<ItemType> propTypes = {ItemType::AddTime, ItemType::Shuffle, ItemType::Hint, ItemType::Freeze, ItemType::Dizzy};
    ItemType type = propTypes[QRandomGenerator::global()->bounded(propTypes.size())];
    
    QPixmap pix;
    switch (type) {
        case ItemType::AddTime: pix = QPixmap(":/images/images/Time.png"); break;
        case ItemType::Shuffle: pix = QPixmap(":/images/images/Shuffle.png"); break;
        case ItemType::Hint:    pix = QPixmap(":/images/images/Hint.png"); break;
        case ItemType::Freeze:  pix = QPixmap(":/images/images/Freeze.png"); break;
        case ItemType::Dizzy:   pix = QPixmap(":/images/images/Dizzy.png"); break;
        default: break;
    }
    Item* prop = new Item(type, pos, rect, pix);
    props.append(prop);
    update();
}

// 绘制道具
void DuoMode::drawProps(QPainter& painter) {
    for (Item* prop : props)
        if (prop->isVisible())
            prop->draw(painter);
}

// paintEvent
void DuoMode::paintEvent(QPaintEvent* event)
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
    QRectF player1Rect = player1->getCord();
    QRectF player2Rect = player2->getCord();
    painter.drawPixmap(player1Rect.x() + 2, player1Rect.y() + 2, 46, 46, player1Pixmap);
    painter.drawPixmap(player2Rect.x() + 2, player2Rect.y() + 2, 46, 46, player2Pixmap);
    drawLinkPath(painter);
}

// 按键处理（双人模式）
void DuoMode::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W) handleMove(0, -1, 1);
    else if (event->key() == Qt::Key_S) handleMove(0, 1, 1);
    else if (event->key() == Qt::Key_A) handleMove(-1, 0, 1);
    else if (event->key() == Qt::Key_D) handleMove(1, 0, 1);
    else if (event->key() == Qt::Key_Up) handleMove(0, -1, 2);
    else if (event->key() == Qt::Key_Down) handleMove(0, 1, 2);
    else if (event->key() == Qt::Key_Left) handleMove(-1, 0, 2);
    else if (event->key() == Qt::Key_Right) handleMove(1, 0, 2);
}

// 处理玩家移动（双人模式版本）
void DuoMode::handleMove(int dx, int dy, int playerId) {
    Player* player = (playerId == 1) ? player1 : player2;
    bool freezeActive = (playerId == 1) ? freezeActive1 : freezeActive2;
    bool dizzyActive = (playerId == 1) ? dizzyActive1 : dizzyActive2;
    
    if (freezeActive) return;
    
    if (dizzyActive) {
        dx = -dx;
        dy = -dy;
    }
    
    linkPath.clear();
    int nx = player->getXInMap() + dx;
    int ny = player->getYInMap() + dy;
    if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) return;
    
    Player* otherPlayer = (playerId == 1) ? player2 : player1;
    if (nx == otherPlayer->getXInMap() && ny == otherPlayer->getYInMap()) {
        return;
    }
    
    if (blocks[ny][nx] && blocks[ny][nx]->getState() != 0) {
        tryActivateBlock(nx, ny, playerId);
        return;
    }
    player->setXInMap(nx);
    player->setYInMap(ny);
    player->getCord().moveTo(topX + nx * blockWidth, topY + ny * blockHeight);
    checkPropCollision(playerId);
    update();
}

// 激活方块逻辑（双人模式版本）
void DuoMode::tryActivateBlock(int bx, int by, int playerId)
{
    linkPath.clear();
    Block* blk = blocks[by][bx];
    Block*& activeBlock = (playerId == 1) ? activeBlock1 : activeBlock2;
    Player* player = (playerId == 1) ? player1 : player2;
    
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
                updateScore(2, playerId);
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

// 判断是否可消除
bool DuoMode::canEliminate(Block* block1, Block* block2) {
    if (!block1 || !block2) return false;
    int x1 = block1->getMapX();
    int y1 = block1->getMapY();
    int x2 = block2->getMapX();
    int y2 = block2->getMapY();
    qDebug() << "canEliminate: block1 mapXY(" << x1 << "," << y1 << ") block2 mapXY(" << x2 << "," << y2 << ")";
    if (block1->getForm() == block2->getForm() && canLink(x1, y1, x2, y2, &linkPath)) {
        block1->setState(0);
        block2->setState(0);
        
        if (hintActive) {
            if (!isHintPairValid()) {
                findHintPair();
            }
        }
        
        update();
        checkGameOver();
        return true;
    }
    return false;
}

// 判断直线是否可连
bool DuoMode::canLinkInLine(int x1, int y1, int x2, int y2)
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

// 判断1拐点是否可连
bool DuoMode::canLinkWithOneCorner(int x1, int y1, int x2, int y2, QVector<QPoint>* path) {
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

// 判断2拐点是否可连
bool DuoMode::canLinkWithTwoCorners(int x1, int y1, int x2, int y2, QVector<QPoint>* path) {
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

// 判断是否可连通
bool DuoMode::canLink(int x1, int y1, int x2, int y2, QVector<QPoint>* path) {
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
void DuoMode::on_exitBtn_clicked()
{
    progressTimer->stop();
    emit exitToMenu();
    this->close();
}

void DuoMode::on_pauseBtn_clicked() {
    pauseGame();
}

// 暂停游戏
// 暂停游戏并显示暂停菜单
void DuoMode::pauseGame() {
    if (isPaused) return;
    isPaused = true;
    if (progressTimer) progressTimer->stop();
    if (propTimer) propTimer->stop();
    if (hintTimer) hintTimer->stop();
    if (freezeTimer1) freezeTimer1->stop();
    if (freezeTimer2) freezeTimer2->stop();
    if (dizzyTimer1) dizzyTimer1->stop();
    if (dizzyTimer2) dizzyTimer2->stop();
    setEnabled(false);
    pauseMenu = new PauseMenu(this);
    connect(pauseMenu, &PauseMenu::continueClicked, this, &DuoMode::onContinueBtnClicked);
    connect(pauseMenu, &PauseMenu::exitClicked, this, &DuoMode::onExitBtnClicked);
    connect(pauseMenu, &PauseMenu::saveClicked, this, &DuoMode::onSaveBtnClicked);
    connect(pauseMenu, &PauseMenu::loadClicked, this, &DuoMode::onLoadBtnClicked);
    pauseMenu->exec();
}

// 恢复游戏
// 恢复游戏状态，关闭暂停菜单
void DuoMode::resumeGame() {
    isPaused = false;
    if (progressTimer) progressTimer->start();
    if (propTimer) propTimer->start();
    if (hintTimer && hintActive) hintTimer->start();
    if (freezeTimer1 && freezeActive1) freezeTimer1->start();
    if (freezeTimer2 && freezeActive2) freezeTimer2->start();
    if (dizzyTimer1 && dizzyActive1) dizzyTimer1->start();
    if (dizzyTimer2 && dizzyActive2) dizzyTimer2->start();
    setEnabled(true);
    if (pauseMenu) pauseMenu->close();
}

// 暂停菜单继续按钮点击槽函数
void DuoMode::onContinueBtnClicked() { resumeGame(); }

// 暂停菜单退出按钮点击槽函数
void DuoMode::onExitBtnClicked() { if (pauseMenu) pauseMenu->close(); close(); }

// 暂停菜单保存按钮点击槽函数
void DuoMode::onSaveBtnClicked() {
    QString path = QFileDialog::getSaveFileName(this, "保存存档", "", "存档文件 (*.txt)");
    if (!path.isEmpty()) saveGame(path, getSaveData());
}

// 暂停菜单加载按钮点击槽函数
void DuoMode::onLoadBtnClicked() {
    QString path = QFileDialog::getOpenFileName(this, "读取存档", "", "存档文件 (*.txt)");
    if (!path.isEmpty()) {
        SaveData data;
        if (loadGame(path, data) && data.mode == GameMode::Duo) {
            applySaveData(data);
            resumeGame();
            if (pauseMenu) pauseMenu->close();
        } else {
        }
    }
}

// 获取存档数据
SaveData DuoMode::getSaveData() const {
    SaveData data;
    data.mode = GameMode::Duo;
    data.timeLeft = timeLeft;
    data.score1 = score1;
    data.score2 = score2;
    data.player1Pos = QPoint(player1->getXInMap(), player1->getYInMap());
    data.player2Pos = QPoint(player2->getXInMap(), player2->getYInMap());
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
void DuoMode::applySaveData(const SaveData& data) {
    timeLeft = data.timeLeft;
    score1 = data.score1;
    score2 = data.score2;
    player1->setXInMap(data.player1Pos.x());
    player1->setYInMap(data.player1Pos.y());
    player1->getCord().moveTo(topX + data.player1Pos.x() * blockWidth, topY + data.player1Pos.y() * blockHeight);
    player2->setXInMap(data.player2Pos.x());
    player2->setYInMap(data.player2Pos.y());
    player2->getCord().moveTo(topX + data.player2Pos.x() * blockWidth, topY + data.player2Pos.y() * blockHeight);
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
            case ItemType::Freeze:  pix = QPixmap(":/images/images/Freeze.png"); break;
            case ItemType::Dizzy:   pix = QPixmap(":/images/images/Dizzy.png"); break;
            default: break;
        }
        Item* prop = new Item(type, data.propPositions[i], rect, pix);
        props.append(prop);
    }
    updateScoreLabels();
    update();
}

// 检查碰撞（双人模式版本）
void DuoMode::checkPropCollision(int playerId) {
    Player* player = (playerId == 1) ? player1 : player2;
    QPoint playerPos(player->getXInMap(), player->getYInMap());
    for (Item* prop : props) {
        if (prop->isVisible()) {
            if (prop->getMapPos() == playerPos) {
                triggerPropEffect(prop->getType(), playerId);
                prop->setVisible(false);
            }
        }
    }
}

// 触发道具效果（双人模式版本）
void DuoMode::triggerPropEffect(ItemType type, int playerId) {
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
        case ItemType::Freeze:
            if (playerId == 1) {
                freezeActive2 = true;
                freezeTimer2->start(3000);
            } else {
                freezeActive1 = true;
                freezeTimer1->start(3000);
            }
            break;
        case ItemType::Dizzy:
            if (playerId == 1) {
                dizzyActive2 = true;
                dizzyTimer2->start(10000);
            } else {
                dizzyActive1 = true;
                dizzyTimer1->start(10000);
            }
            break;
        default: break;
    }
    update();
}

// 检查当前高亮的Hint方块是否仍然有效
bool DuoMode::isHintPairValid() {
    if (hintBlock1 == QPoint(-1, -1) || hintBlock2 == QPoint(-1, -1)) return false;
    
    Block* b1 = blocks[hintBlock1.y()][hintBlock1.x()];
    Block* b2 = blocks[hintBlock2.y()][hintBlock2.x()];
    
    if (!b1 || !b2 || b1->getState() == 0 || b2->getState() == 0) return false;
    
    return b1->getForm() == b2->getForm() && canLink(b1->getMapX(), b1->getMapY(), b2->getMapX(), b2->getMapY());
}

// 查找可消除对用于Hint
void DuoMode::findHintPair() {
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

// 鼠标点击事件处理，支持Flash道具的瞬间移动功能（双人模式）
void DuoMode::mousePressEvent(QMouseEvent* event) {
    if (!flashActive1 && !flashActive2) return;
    
    QPoint pos = event->pos();
    int mx = (pos.x() - topX) / blockWidth;
    int my = (pos.y() - topY) / blockHeight;
    
    if (mx < 0 || mx >= cols || my < 0 || my >= rows) return;
    
    if (mx == player2->getXInMap() && my == player2->getYInMap()) {
        return;
    }
    
    if (!blocks[my][mx] || blocks[my][mx]->getState() == 0) {
        if (flashActive1) {
            player1->setXInMap(mx);
            player1->setYInMap(my);
            player1->getCord().moveTo(topX + mx * blockWidth, topY + my * blockHeight);
            checkPropCollision(1);
        } else if (flashActive2) {
            player2->setXInMap(mx);
            player2->setYInMap(my);
            player2->getCord().moveTo(topX + mx * blockWidth, topY + my * blockHeight);
            checkPropCollision(2);
        }
        update();
    } else {
        static const int dx[4] = {0, 0, -1, 1};
        static const int dy[4] = {-1, 1, 0, 0};
        for (int d = 0; d < 4; ++d) {
            int nx = mx + dx[d], ny = my + dy[d];
            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows && 
                (!blocks[ny][nx] || blocks[ny][nx]->getState() == 0) &&
                !(nx == player1->getXInMap() && ny == player1->getYInMap()) &&
                !(nx == player2->getXInMap() && ny == player2->getYInMap())) {
                
                if (flashActive1) {
                    player1->setXInMap(nx);
                    player1->setYInMap(ny);
                    player1->getCord().moveTo(topX + nx * blockWidth, topY + ny * blockHeight);
                    tryActivateBlock(mx, my, 1);
                    checkPropCollision(1);
                } else if (flashActive2) {
                    player2->setXInMap(nx);
                    player2->setYInMap(ny);
                    player2->getCord().moveTo(topX + nx * blockWidth, topY + ny * blockHeight);
                    tryActivateBlock(mx, my, 2);
                    checkPropCollision(2);
                }
                update();
                break;
            }
        }
    }
}
